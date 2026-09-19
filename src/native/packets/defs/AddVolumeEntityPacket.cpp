#include "packets/Nbt.h"
#include "packets/Packet.h"

#include <string>

namespace {
using namespace bpx::schema;

Layout volume(std::size_t netId) {
    return {
        string("mJsonIdentifier", 0x18),
        string("mInstanceName", 0x38),
        blockPos("mMinBounds", 0x58),
        blockPos("mMaxBounds", 0x64),
        i32("mDimensionType", 0x70),
        u32("mEntityNetId", netId),
    };
}
}

BPX_PACKET("AddVolumeEntityPacket") {
    // before 1.18.30 the reader rejects the default packet's empty CompoundTag, so it gets bytes: varuint net id,
    // components NBT, [1.18.11+ identifier, instance name], [1.17.30+ engine version; an empty one is left unread]
    p.since("1.18.30");
    {
        namespace n = bpx::packets::nbt;
        std::string const components =
            n::root(n::compoundTag("minecraft:bounds", n::intTag("min_x", 0) + n::intTag("max_x", 16)) + n::stringTag("identifier", "minecraft:fog_volume"));
        std::string const names = "\x14minecraft:fog_volume\x04" "fog1";
        struct Era {
            char const* since;
            char const* until;
            std::string tail;
        };
        for (auto const& [since, until, tail] : {Era{"1.17.0", "1.17.30", ""}, Era{"1.17.30", "1.18.11", "\x07" "1.17.30"},
                                                 Era{"1.18.11", "1.18.30", names + "\x07" "1.18.10"}}) {
            p.payload(since, until, "id 1", "\x01" + components + tail);
            p.payload(since, until, "id 300", "\xac\x02" + components + tail);
        }
    }
    p.shape("1.18.30", {u32("mEntityNetId", 0x0), string("mJsonIdentifier", 0x20), string("mInstanceName", 0x40),
                        blockPos("mMinBounds", 0x60), blockPos("mMaxBounds", 0x6C), i32("mDimensionType", 0x78)});
    p.shape("1.20.61", volume(0xE8));
    p.shape("1.21.30", volume(0xF0));
    p.shape("1.21.60", volume(0x98));

    // the game reads back neither an empty identifier / instance name nor the default dimension (Undefined -1)
    p.baseline("mJsonIdentifier", "minecraft:fog_volume");
    p.baseline("mInstanceName", "volume_1");
    p.baseline("mDimensionType", 0);

    p.values("mEntityNetId", {0, 1, 12345, std::int64_t{4294967295LL}});
    p.values("mJsonIdentifier", {"minecraft:fog_volume", "custom:trigger_zone"});
    p.values("mInstanceName", {"volume_1", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mMinBounds", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}}});
    p.values("mMaxBounds", {Object{{"x", 16}, {"y", 80}, {"z", 16}}, Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mDimensionType", {0, 1, 2}); // Overworld, Nether, TheEnd
}
