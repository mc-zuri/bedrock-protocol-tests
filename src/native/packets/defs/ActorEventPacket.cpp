#include "packets/Packet.h"

#include <climits>
#include <cstdint>
#include <initializer_list>

namespace {
using namespace bpx::schema;

List events(std::initializer_list<std::pair<int, int>> ranges) {
    List out;
    for (auto [from, to] : ranges)
        for (int v = from; v <= to; ++v) out.push_back(v);
    return out;
}
}

BPX_PACKET("ActorEventPacket") {
    p.shape("1.16.201", {
        u64("mRuntimeId", 0x0),
        u8("mEventId", 0x8),
        i32("mData", 0xC),
    });
    p.shape("1.26.20", {
        u64("mRuntimeId", 0x0),
        u8("mEventId", 0x8),
        i32("mData", 0xC),
        optional("mFireAtPosition", 0x10, 0xC, vec3("value", 0)),
    });

    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mData", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("mFireAtPosition", {Value(), Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}},
                                 Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});

    // ActorEvent: None 0 .. ShakeWetness 8, (UseItem 9 until 1.20), EatGrass 10 .. Shake 39, Feed 57, BabyAge 60 ..
    p.values("1.16.201", "mEventId", events({{0, 39}, {57, 57}, {60, 74}}));
    p.values("1.17.30", "mEventId", events({{0, 39}, {57, 57}, {60, 76}}));          // + LandedOnGround, ActorGrowUp
    p.values("1.18.30", "mEventId", events({{0, 39}, {57, 57}, {60, 77}}));          // + VibrationDetected
    p.values("1.19.50", "mEventId", events({{0, 39}, {57, 57}, {60, 78}}));          // + DrinkMilk
    p.values("1.20.30", "mEventId", events({{0, 8}, {10, 39}, {57, 57}, {60, 78}})); // UseItem gone
    p.values("1.21.50", "mEventId", events({{0, 8}, {10, 39}, {57, 57}, {60, 74}, {76, 78}})); // LandedOnGround gone
    p.values("1.21.120", "mEventId", events({{0, 8}, {10, 39}, {57, 57}, {60, 74}, {76, 79}})); // + ShakeWetnessStop
    p.values("1.21.130", "mEventId", events({{0, 8}, {10, 39}, {57, 57}, {60, 74}, {76, 80}})); // + KineticDamageDealt
    p.values("1.26.20", "mEventId", events({{0, 8}, {10, 39}, {57, 57}, {60, 74}, {76, 81}}));  // + HurtWithoutReceivingDamage
}
