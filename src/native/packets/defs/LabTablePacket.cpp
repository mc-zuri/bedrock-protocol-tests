#include "packets/Packet.h"

BPX_PACKET("LabTablePacket") {
    p.shape("1.16.201", {u8("mType", 0x0), blockPos("mPos", 0x4), u8("mReaction", 0x10)});

    p.values("mType", {0, 1, 2}); // StartCombine, StartReaction, Reset
    p.values("mPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                      Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    // LabTableReactionType: None, IceBomb, Bleach, ElephantToothpaste, Fertilizer, HeatBlock, MagnesiumSalts, MiscFire,
    // MiscExplosion, MiscLava, MiscMystical, MiscSmoke, MiscLargeSmoke
    p.values("mReaction", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
}
