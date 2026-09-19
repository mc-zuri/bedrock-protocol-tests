#include "packets/Packet.h"

namespace {
using namespace bpx::schema;
Object damages(int a, int b, int c, int d, int e) { return Object{{"helmet", a}, {"chest", b}, {"legs", c}, {"feet", d}, {"body", e}}; }
Object pair(int slot, int damage) { return Object{{"mArmorSlot", slot}, {"mDamage", damage}}; }
}

BPX_PACKET("PlayerArmorDamagePacket") {
    Field const array = structure("mDamages", 0x4, {i16("helmet", 0x0), i16("chest", 0x2), i16("legs", 0x4), i16("feet", 0x6),
                                                    i16("body", 0x8)});
    p.shape("1.16.201", {u8("mSlots", 0x0), array});
    p.shape("1.21.111", {vector("mSlotAndDamagePairs", 0x0, 8, structure("value", 0, {i32("mArmorSlot", 0x0), i16("mDamage", 0x4)}))});

    // a damage only goes with its bit
    for (int bit = 0; bit < 5; ++bit) {
        int d[5] = {0, 0, 0, 0, 0};
        d[bit]   = 7;
        p.scenario(bit == 4 ? "1.21.20" : "0.0.0", "slot " + std::to_string(bit),
                   Object{{"mSlots", 1 << bit}, {"mDamages", damages(d[0], d[1], d[2], d[3], d[4])}});
    }
    p.scenario("four slots", Object{{"mSlots", 15}, {"mDamages", damages(1, 2, 3, 4, 0)}});
    p.scenario("1.21.20", "all slots", Object{{"mSlots", 31}, {"mDamages", damages(1, 2, 3, 4, 500)}});
    p.values("mSlotAndDamagePairs", {List{}, List{pair(0, 1)}, List{pair(1, 2), pair(2, 3), pair(3, 4), pair(4, 500)}});
}
