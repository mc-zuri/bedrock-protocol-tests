#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;
Object xyz(double x, double y, double z) { return Object{{"x", x}, {"y", y}, {"z", z}}; }
}

BPX_PACKET("CorrectPlayerMovePredictionPacket") {
    p.shape("1.16.201", {
        vec3("mPos", 0x0),
        vec3("mPosDelta", 0xC),
        u64("mTick", 0x18),
        boolean("mOnGround", 0x20),
    });
    p.shape("1.20.80", {
        vec3("mPos", 0x0),
        vec3("mPosDelta", 0xC),
        vec2("mVehicleRotation", 0x18),
        u64("mTick", 0x20),
        boolean("mOnGround", 0x28),
        u8("mPredictionType", 0x29),
    });
    p.shape("1.21.20", {
        vec3("mPos", 0x0),
        vec3("mPosDelta", 0xC),
        vec2("mVehicleRotation", 0x18),
        optional("mVehicleAngularVelocity", 0x20, 4, f32("value", 0)),
        u64("mTick", 0x28),
        boolean("mOnGround", 0x30),
        u8("mPredictionType", 0x31),
    });

    p.values("mPos", {xyz(0.0, 64.0, 0.0), xyz(-123.5, 70.25, 456.75), xyz(1000.0, -60.0, -1000.0)});
    p.values("mPosDelta", {xyz(0.0, 0.0, 0.0), xyz(0.1, -0.0784, 0.25)});
    p.values("mVehicleRotation", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 45.0}, {"y", -90.0}}});
    p.values("mVehicleAngularVelocity", {Value(), 0.0, 1.5, -2.25});
    p.values("mTick", {0, 1, 1234, std::int64_t{1000000000000LL}});
    p.values("mOnGround", {false, true});
    p.values("mPredictionType", {0, 1}); // RewindType: Player, Vehicle
}
