#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("GraphicsOverrideParameterPacket") {
    namespace e = bpx::packets::enums;
    p.since("1.21.120");
    p.shape("1.21.120", {string("mBiomeID", 0x40), u8("mParameterId", 0x60), boolean("mResetParameter", 0x61)});
    p.shape("1.26.0", {optional("mFloatValue", 0x40, 0x4, f32("value", 0)), optional("mVec3Value", 0x48, 0xC, vec3("value", 0)),
                       string("mBiomeID", 0x58), u8("mParameterId", 0x78), boolean("mResetParameter", 0x79)});
    p.shape("1.26.30", {optional("mFloatValue", 0x40, 0x4, f32("value", 0)), optional("mVec3Value", 0x48, 0xC, vec3("value", 0)),
                        string("mBiomeID", 0x58), optional("mPlayerID", 0x78, 0x20, string("value", 0)), u8("mParameterId", 0xA0),
                        boolean("mResetParameter", 0xA1)});

    p.values("mBiomeID", {"", "minecraft:plains", "minecraft:the_end"});
    // the reader rejects parameter types its version doesn't define (SkyZenithColor 0 .. OrbitalOffsetDegrees 51)
    p.values("mParameterId", {0});
    p.values("1.21.130", "mParameterId", e::range(0, 9));
    p.values("1.26.0", "mParameterId", e::range(0, 25));
    p.values("1.26.10", "mParameterId", e::range(0, 48));
    p.values("1.26.20", "mParameterId", e::range(0, 51));
    p.values("mResetParameter", {false, true});
    p.values("mFloatValue", {Value(), 0.0, 0.5, -2.25, 1000.0});
    p.values("mVec3Value", {Value(), Object{{"x", 1.0}, {"y", 0.5}, {"z", 0.0}}, Object{{"x", -1.0}, {"y", 100.0}, {"z", 0.25}}});
    p.values("mPlayerID", {Value(), "", "-12884901887"});
}
