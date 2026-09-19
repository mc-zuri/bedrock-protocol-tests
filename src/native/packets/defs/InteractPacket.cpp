#include "packets/Packet.h"

#include <cstdint>

namespace {
bpx::schema::Object xyz(double x, double y, double z) { return {{"x", x}, {"y", y}, {"z", z}}; }
}

BPX_PACKET("InteractPacket") {
    p.shape("1.16.201", {u8("mAction", 0x0), u64("mTargetId", 0x8), vec3("mPos", 0x10)});
    p.shape("1.21.130", {u8("mAction", 0x0), u64("mTargetId", 0x8), optional("mPos", 0x10, 0xC, vec3("value", 0))});

    p.values("mAction", {0, 3, 4, 5, 6}); // Invalid, StopRiding, InteractUpdate, NpcOpen, OpenInventory
    p.values("mTargetId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mPos", {xyz(0.5, 65.0, -0.5), xyz(-123.25, -60.0, 30000.75)});
    // 1.21.50 .. 1.21.124: the position only goes with StopRiding / InteractUpdate
    p.values("1.21.50", "mPos", {xyz(0.0, 0.0, 0.0)});
    for (int a : {3, 4}) p.scenario("1.21.50", "1.21.130", "position with the action", {{"mAction", a}, {"mPos", xyz(-123.25, 70.0, 30000.75)}});
    p.values("1.21.130", "mPos", {Value(), xyz(0.5, 65.0, -0.5), xyz(-123.25, -60.0, 30000.75)});
}
