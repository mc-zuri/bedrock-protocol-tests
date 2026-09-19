#include "packets/Nbt.h"
#include "packets/Packet.h"

#include <cstdint>
#include <vector>

namespace {
using namespace bpx::schema;

Field optF(char const* n, std::size_t at) { return optional(n, at, 4, f32("value", 0)); }
Field optB(char const* n, std::size_t at) { return optional(n, at, 1, boolean("value", 0)); }
Field optV2(char const* n, std::size_t at) { return optional(n, at, 8, vec2("value", 0)); }

Field preset(bool v26_50) {
    std::vector<Field> m{string("mName", 0x0), string("mInheritFrom", 0x20), optF("mPosX", 0x40), optF("mPosY", 0x48), optF("mPosZ", 0x50),
                      optF("mRotX", 0x58), optF("mRotY", 0x60), optF("mCameraRotationSpeed", 0x68), optB("mSnapToTarget", 0x70),
                      optV2("mHorizontalRotationLimit", 0x74), optV2("mVerticalRotationLimit", 0x80), optB("mContinueTargeting", 0x8C),
                      optF("mTrackingRadius", 0x90), optV2("mViewOffset", 0x98), optional("mEntityOffset", 0xA4, 0xC, vec3("value", 0)),
                      optF("mRadius", 0xB4), optF("mYawLimitMin", 0xBC), optF("mYawLimitMax", 0xC4),
                      optional("mListener", 0xCC, 1, u8("value", 0)), optB("mPlayerEffects", 0xCE),
                      optional("mControlScheme", 0x118, 1, u8("value", 0))};
    if (v26_50) {
        m.push_back(boolean("mApplyInheritedStartingRotation", 0x120));
        m.push_back(optV2("mStartingRot", 0x124));
    }
    return structure("value", 0, m);
}

Layout presets(bool v26_50) {
    return {vector("mPresets", 0x0, v26_50 ? 0x130 : 0x120, preset(v26_50)),
            vector("mLists", 0x18, 0x38, structure("value", 0, {string("mName", 0x0), vector("mPresetIndices", 0x20, 4, u32("value", 0))}))};
}

Object xy(double x, double y) { return Object{{"x", x}, {"y", y}}; }
}

BPX_PACKET("CameraPresetsPacket") {
    // 1.19.70 .. 1.20.15 send the presets as NBT and the reader rejects the default empty compound
    p.since("1.20.30");
    {
        namespace n = bpx::packets::nbt;
        p.payload("1.19.70", "1.20.30", "no presets", n::root(n::compoundList("presets", {})));
        p.payload("1.19.70", "1.20.30", "presets",
                  n::root(n::compoundList("presets", {n::stringTag("identifier", "minecraft:free") + n::stringTag("inherit_from", "") +
                                                          n::floatTag("pos_x", 0.0f) + n::floatTag("pos_y", 64.0f) + n::floatTag("pos_z", 0.0f) +
                                                          n::floatTag("rot_x", 0.0f) + n::floatTag("rot_y", 90.0f),
                                                      n::stringTag("identifier", "minecraft:first_person") + n::stringTag("inherit_from", "")})));
    }
    p.shape("1.20.30", {}); // the older preset types are not built
    p.shape("1.21.90", presets(false));
    p.shape("1.26.50", presets(true));

    Object const bare{{"mName", "minecraft:free"}};
    Object const full{{"mName", "custom:orbit"},
                      {"mInheritFrom", "minecraft:third_person"},
                      {"mPosX", 1.5},
                      {"mPosY", 70.0},
                      {"mPosZ", -3.25},
                      {"mRotX", 30.0},
                      {"mRotY", -90.0},
                      {"mCameraRotationSpeed", 2.0},
                      {"mSnapToTarget", true},
                      {"mHorizontalRotationLimit", xy(-45.0, 45.0)},
                      {"mVerticalRotationLimit", xy(-30.0, 60.0)},
                      {"mContinueTargeting", false},
                      {"mTrackingRadius", 50.0},
                      {"mViewOffset", xy(0.5, 1.0)},
                      {"mEntityOffset", Object{{"x", 0.0}, {"y", 1.62}, {"z", 0.0}}},
                      {"mRadius", 4.0},
                      {"mYawLimitMin", -90.0},
                      {"mYawLimitMax", 90.0},
                      {"mListener", 1},
                      {"mPlayerEffects", true},
                      {"mControlScheme", 2}};
    List list{List{}, List{bare}, List{full}, List{bare, full}};
    // AudioListener: Camera, Player; ControlScheme::Scheme: LockedPlayerRelativeStrafe .. PlayerRelativeStrafe
    for (int l : {0, 1}) list.push_back(List{Object{{"mName", "custom:listener"}, {"mListener", l}}});
    for (int s = 0; s <= 4; ++s) list.push_back(List{Object{{"mName", "custom:scheme"}, {"mControlScheme", s}}});
    list.push_back(List{Object{{"mName", "custom:start"}, {"mApplyInheritedStartingRotation", true}, {"mStartingRot", xy(15.0, 180.0)}}});
    p.values("mPresets", list);
    p.scenario("1.21.90", "presets with lists",
               {{"mPresets", List{bare, full}},
                {"mLists", List{Object{{"mName", "cycle"}, {"mPresetIndices", List{0, 1}}}, Object{{"mName", "one"}, {"mPresetIndices", List{1}}}}}});
}
