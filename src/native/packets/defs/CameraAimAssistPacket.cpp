#include "packets/Packet.h"

BPX_PACKET("CameraAimAssistPacket") {
    p.since("1.21.30");
    p.shape("1.21.30", {vec2("mViewAngle", 0x0), f32("mDistance", 0x8), u8("mTargetMode", 0xC), u8("mAction", 0xD)});
    Layout l{string("mPresetId", 0x0), vec2("mViewAngle", 0x20), f32("mDistance", 0x28), u8("mTargetMode", 0x2C), u8("mAction", 0x2D)};
    p.shape("1.21.50", l);
    l.push_back(boolean("mShowDebugRender", 0x2E));
    p.shape("1.21.100", l);

    p.baseline("mPresetId", "minecraft:aim_assist_default");
    p.baseline("mDistance", 8.0);
    p.values("mPresetId", {"minecraft:aim_assist_default", "custom:sniper"});
    p.values("mViewAngle", {Object{{"x", 10.0}, {"y", 10.0}}, Object{{"x", 45.0}, {"y", 90.0}}});
    p.values("mDistance", {1.0, 8.0, 16.0});
    p.values("mTargetMode", {0, 1}); // Angle, Distance
    p.values("mAction", {0, 1});     // Set, Clear
    p.values("mShowDebugRender", {false, true});
}
