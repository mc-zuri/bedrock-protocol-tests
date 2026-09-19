#include "packets/Captures.h"
#include "packets/Nbt.h"
#include "packets/Packet.h"

#include <iterator>
#include <string>

namespace {
// "COMPRESSED", a dictionary of one entry (the byte 0xFF), then the NBT with each 0xFF as a reference to it
std::string compress(std::string const& nbt) {
    std::string out("COMPRESSED\x01\x00\x01\xff", 14);
    for (char c : nbt) {
        if (static_cast<unsigned char>(c) == 0xFF) out += std::string("\xff\x00\x00", 3);
        else out += c;
    }
    return out;
}
}

BPX_PACKET("CompressedBiomeDefinitionList") {
    // before 1.20.61 the writer compresses a CompoundTag itself, so those builds get payloads
    p.since("1.20.61");
    p.until("1.21.80");
    p.shape("1.20.61", {string("mCompressedBiomeData", 0x18)});

    char const* const builds[] = {"1.19.80", "1.20.0", "1.20.10", "1.20.15", "1.20.30", "1.20.40", "1.20.50", "1.20.61", "1.20.71",
                                  "1.20.80", "1.21.0",  "1.21.2",  "1.21.20", "1.21.30", "1.21.42", "1.21.50", "1.21.60", "1.21.70", "1.21.80"};
    p.rewrites("it compresses the biome compound its own way (a dictionary of the repeated names)");
    // the reader rejects the default empty compound; each build gets its own captured biomes
    for (std::size_t i = 0; i + 1 < std::size(builds); ++i) {
        auto nbt = bpx::packets::captures::payload(builds[i], "biome_definition_list");
        if (!nbt) continue;
        std::string const c = compress(*nbt);
        if (bpx::Version::parse(builds[i]) < bpx::Version::parse("1.20.61")) {
            p.payload(builds[i], builds[i + 1], "biomes", bpx::packets::nbt::varuint(c.size()) + c);
            continue;
        }
        p.baseline(builds[i], "mCompressedBiomeData", c);
    }
    // the reader keeps only the decoded compound (its writer writes only the string), so a BiomeDefinitionList
    // packet writes that compound back to check it
    p.checkedVia("1.20.61", "1.21.60", "BiomeDefinitionList", 0x0, 0x0, 0x18, "biome_definition_list");
    p.checkedVia("1.21.60", "1.21.80", "BiomeDefinitionListPacket", 0x0, 0x0, 0x18, "biome_definition_list");
}
