#include "packets/Packet.h"

BPX_PACKET("BiomeDefinitionListPacket") {
    p.since("1.21.80");
    // 1.21.60 .. 1.21.70 send a CompoundTag the reader rejects empty, so only the captured packet is read there
    p.captured("biome_definition_list");
    // the biome definition maps are left empty; only the string list is built
    p.shape("1.21.80", {vector("mStrings", 0x80, 0x20, string("value", 0))});
    p.shape("1.26.20", {vector("mStrings", 0x50, 0x20, string("value", 0))});

    p.values("mStrings", {List{}, List{"plains"}, List{"plains", "minecraft:desert", "\xc2\xa7" "aunicode \xe2\x9c\x93"}});
}
