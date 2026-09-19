#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("HurtArmorPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {i32("mCause", 0x0), i32("mDmg", 0x4)});
    p.shape("1.17.30", {i32("mCause", 0x0), i32("mDmg", 0x4), u32("mArmorSlots", 0x8)});

    p.values("mCause", e::range(0, 26));           // Override .. Temperature
    p.values("1.17.0", "mCause", e::range(0, 30));  // + Freezing, Stalactite, Stalagmite, RamAttack
    p.values("1.19.1", "mCause", e::range(0, 31));  // + SonicBoom
    p.values("1.20.50", "mCause", e::range(0, 33)); // + Campfire, SoulCampfire
    p.values("1.21.50", "mCause", e::range(0, 34)); // + MaceSmash
    p.values("1.21.130", "mCause", e::range(0, 35)); // + Dehydration
    p.values("1.26.10", "mCause", e::range(0, 34)); // Dehydration removed
    p.values("mDmg", {0, 1, 20, 1000});
    // bitset: helmet, chest, legs, feet, + body from 1.21.20; each slot, then all of them
    p.values("mArmorSlots", {0, 1, 2, 4, 8, 15});
    p.values("1.21.20", "mArmorSlots", {0, 1, 2, 4, 8, 16, 15, 31});
}
