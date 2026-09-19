#include "packets/Nbt.h"
#include "packets/Packet.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace bpx::schema;

Field opt(std::string name, std::size_t offset, std::size_t size, std::vector<Field> members) {
    return optional(std::move(name), offset, size, structure("value", 0, std::move(members)));
}
Field optBool(std::string name, std::size_t offset) { return optional(std::move(name), offset, 1, boolean("value", 0)); }

Layout cameraInstruction() {
    return {
        opt("mAttachToEntity", 0x0, 0x10, {i64("mAttachToEntityId", 0x0), i32("mActorLocator", 0x8)}),
        opt("mFade", 0x18, 0x20,
            {opt("mTime", 0x0, 0xC, {f32("mFadeInTime", 0x0), f32("mHoldTime", 0x4), f32("mFadeOutTime", 0x8)}),
             opt("mColor", 0x10, 0xC, {f32("mRed", 0x0), f32("mGreen", 0x4), f32("mBlue", 0x8)})}),
        opt("mFieldOfView", 0x3C, 0x10, {f32("mFov", 0x0), boolean("mFovClear", 0x4), f32("mFovEaseTime", 0x8), i32("mFovEaseType", 0xC)}),
        opt("mSet", 0x50, 0x5C,
            {u32("mPresetIndex", 0x0),
             opt("mEase", 0x4, 0x8, {i32("mEasingType", 0x0), f32("mEasingTime", 0x4)}),
             opt("mPos", 0x10, 0xC, {vec3("mPos", 0x0)}),
             opt("mRot", 0x20, 0x8, {f32("mRotX", 0x0), f32("mRotY", 0x4)}),
             opt("mFacing", 0x2C, 0xC, {vec3("mFacingPos", 0x0)}),
             opt("mViewOffset", 0x3C, 0x8, {f32("mViewOffsetX", 0x0), f32("mViewOffsetY", 0x4)}),
             opt("mEntityOffset", 0x48, 0xC, {f32("mEntityOffsetX", 0x0), f32("mEntityOffsetY", 0x4), f32("mEntityOffsetZ", 0x8)}),
             optBool("mDefault", 0x58),
             boolean("mRemoveIgnoreStartingValuesComponent", 0x5A)}),
        opt("mSpline", 0xB0, 0x70,
            {string("mSplineIdentifier", 0x0), f32("mTotalTime", 0x20), u8("mCurveType", 0x24), boolean("mLoadFromJson", 0x25),
             vector("mCurve", 0x28, 0xC, vec3("value", 0)),
             vector("mProgressKeyFrames", 0x40, 0xC,
                    structure("value", 0, {f32("mProgressKeyFrameValue", 0x0), f32("mProgressKeyFrameTime", 0x4),
                                           i32("mProgressKeyFramesEasingFunc", 0x8)})),
             vector("mSplineRotationOption", 0x58, 0x14,
                    structure("value", 0, {vec3("mRotationKeyFrameValue", 0x0), f32("mRotationKeyFrameTime", 0xC),
                                           i32("mRotationKeyFramesEasingFunc", 0x10)}))}),
        opt("mTarget", 0x128, 0x18, {optional("mTargetCenterOffset", 0x0, 0xC, vec3("value", 0)), i64("mTargetActorId", 0x10)}),
        optBool("mClear", 0x148),
        optBool("mDetachFromEntity", 0x14A),
        optBool("mRemoveTarget", 0x14C),
    };
}

Field setInstruction(std::size_t at, char era) {
    std::vector<Field> m{u32("mPresetIndex", 0x0),
                         opt("mEase", 0x4, 0x8, {i32("mEasingType", 0x0), f32("mEasingTime", 0x4)}),
                         opt("mPos", 0x10, 0xC, {vec3("mPos", 0x0)}),
                         opt("mRot", 0x20, 0x8, {f32("mRotX", 0x0), f32("mRotY", 0x4)}),
                         opt("mFacing", 0x2C, 0xC, {vec3("mFacingPos", 0x0)})};
    std::size_t size = 0x40;
    if (era == 'a') {
        m.push_back(optBool("mDefault", 0x3C));
    } else {
        m.push_back(opt("mViewOffset", 0x3C, 0x8, {f32("mViewOffsetX", 0x0), f32("mViewOffsetY", 0x4)}));
        if (era == 'b') {
            m.push_back(optBool("mDefault", 0x48));
            size = 0x4C;
        } else {
            m.push_back(opt("mEntityOffset", 0x48, 0xC, {f32("mEntityOffsetX", 0x0), f32("mEntityOffsetY", 0x4), f32("mEntityOffsetZ", 0x8)}));
            m.push_back(optBool("mDefault", 0x58));
            if (era != 'c') m.push_back(boolean("mRemoveIgnoreStartingValuesComponent", 0x5A));
            size = 0x5C;
        }
    }
    return opt("mSet", at, size, m);
}
Field fade(std::size_t at) {
    return opt("mFade", at, 0x20,
               {opt("mTime", 0x0, 0xC, {f32("mFadeInTime", 0x0), f32("mHoldTime", 0x4), f32("mFadeOutTime", 0x8)}),
                opt("mColor", 0x10, 0xC, {f32("mRed", 0x0), f32("mGreen", 0x4), f32("mBlue", 0x8)})});
}
Field target(std::size_t at) {
    return opt("mTarget", at, 0x18, {optional("mTargetCenterOffset", 0x0, 0xC, vec3("value", 0)), i64("mTargetActorId", 0x10)});
}
Field fov(std::size_t at) {
    return opt("mFieldOfView", at, 0x10, {f32("mFov", 0x0), boolean("mFovClear", 0x4), f32("mFovEaseTime", 0x8), i32("mFovEaseType", 0xC)});
}

// eras: 'a' 1.20.30, 'b' 1.21.20, 'c' 1.21.42, 'd' 1.21.90, 'e' 1.21.100
Layout older(char era) {
    if (era == 'a') return {setInstruction(0x0, 'a'), optBool("mClear", 0x44), fade(0x48)};
    if (era == 'b') return {setInstruction(0x0, 'b'), target(0x50), optBool("mRemoveTarget", 0x70), optBool("mClear", 0x72), fade(0x74)};
    Layout l{setInstruction(0x0, era), target(0x60), optBool("mRemoveTarget", 0x80), optBool("mClear", 0x82), fade(0x84)};
    if (era == 'e') l.push_back(fov(0xA8));
    return l;
}

Layout instruction121() {
    return {opt("mAttachToEntity", 0x0, 0x8, {i64("mAttachToEntityId", 0x0)}),
            fade(0x10),
            fov(0x34),
            setInstruction(0x48, 'd'),
            opt("mSpline", 0xA8, 0x50,
                {f32("mTotalTime", 0x0), u8("mCurveType", 0x4), vector("mCurve", 0x8, 0xC, vec3("value", 0)),
                 vector("mProgressKeyFrames", 0x20, 0x8,
                        structure("value", 0, {f32("mProgressKeyFrameValue", 0x0), f32("mProgressKeyFrameTime", 0x4)})),
                 vector("mSplineRotationOption", 0x38, 0x10,
                        structure("value", 0, {vec3("mRotationKeyFrameValue", 0x0), f32("mRotationKeyFrameTime", 0xC)}))}),
            target(0x100),
            optBool("mClear", 0x120),
            optBool("mDetachFromEntity", 0x122),
            optBool("mRemoveTarget", 0x124)};
}

Object xyz(double x, double y, double z) { return Object{{"x", x}, {"y", y}, {"z", z}}; }
}

BPX_PACKET("CameraInstructionPacket") {
    // 1.19.70 .. 1.20.15 send the instruction as NBT and the reader rejects the default empty compound
    p.since("1.20.30");
    {
        namespace n = bpx::packets::nbt;
        p.payload("1.19.70", "1.20.30", "clear", n::root(n::byteTag("clear", 1)));
        p.payload("1.19.70", "1.20.30", "set",
                  n::root(n::compoundTag("set", n::intTag("preset", 0) + n::compoundTag("rot", n::floatTag("x", 30.0f) + n::floatTag("y", 90.0f)))));
        p.payload("1.19.70", "1.20.30", "fade",
                  n::root(n::compoundTag("fade", n::compoundTag("time", n::floatTag("fadeIn", 0.5f) + n::floatTag("hold", 1.0f) + n::floatTag("fadeOut", 0.5f)) +
                                                     n::compoundTag("color", n::floatTag("r", 1.0f) + n::floatTag("g", 0.0f) + n::floatTag("b", 0.0f)))));
    }
    p.shape("1.20.30", older('a'));
    p.shape("1.21.20", older('b'));
    p.shape("1.21.42", older('c'));
    p.shape("1.21.90", older('d'));
    p.shape("1.21.100", older('e'));
    p.shape("1.21.120", instruction121());
    p.shape("1.26.0", cameraInstruction());

    p.values("mClear", {false, true});
    p.values("mDetachFromEntity", {false, true});
    p.values("mRemoveTarget", {false, true});
    p.values("mAttachToEntity", {Object{{"mAttachToEntityId", 1}}, Object{{"mAttachToEntityId", -4294967295LL}}});
    p.values("mFade", {Object{},
                       Object{{"mTime", Object{{"mFadeInTime", 0.5}, {"mHoldTime", 1.0}, {"mFadeOutTime", 0.5}}}},
                       Object{{"mColor", Object{{"mRed", 1.0}, {"mGreen", 0.0}, {"mBlue", 0.25}}}},
                       Object{{"mTime", Object{{"mFadeInTime", 0.0}, {"mHoldTime", 10.0}, {"mFadeOutTime", 2.5}}},
                              {"mColor", Object{{"mRed", 0.0}, {"mGreen", 0.0}, {"mBlue", 0.0}}}}});
    p.values("mTarget", {Object{{"mTargetActorId", 1}},
                         Object{{"mTargetCenterOffset", xyz(0.0, 1.5, 0.0)}, {"mTargetActorId", -4294967295LL}}});
    {
        List fov, set, spline, attach;
        for (int e = 0; e <= 31; ++e) { // EasingType: Linear 0 .. InOutElastic 31 (Count / Invalid not sent)
            fov.push_back(Object{{"mFov", 30.0 + e * 2.5}, {"mFovClear", e % 2 == 1}, {"mFovEaseTime", 0.5 * e}, {"mFovEaseType", e}});
            set.push_back(Object{{"mPresetIndex", e % 4}, {"mEase", Object{{"mEasingType", e}, {"mEasingTime", 1.5}}}});
            spline.push_back(Object{{"mSplineIdentifier", ""}, {"mTotalTime", 4.0}, {"mCurveType", e % 2}, {"mLoadFromJson", false},
                                    {"mCurve", List{xyz(0.0, 64.0, 0.0), xyz(10.0, 70.0, -5.5)}},
                                    {"mProgressKeyFrames", List{Object{{"mProgressKeyFrameValue", 0.0}, {"mProgressKeyFrameTime", 0.0},
                                                                       {"mProgressKeyFramesEasingFunc", e}}}},
                                    {"mSplineRotationOption", List{Object{{"mRotationKeyFrameValue", xyz(0.0, 90.0, 0.0)},
                                                                          {"mRotationKeyFrameTime", 1.0},
                                                                          {"mRotationKeyFramesEasingFunc", e}}}}});
        }
        for (int l = 0; l <= 8; ++l) // ActorLocation: Feet 0 .. Mouth 8
            attach.push_back(Object{{"mAttachToEntityId", 12345}, {"mActorLocator", l}});
        p.values("mFieldOfView", fov);
        for (auto& a : attach) p.scenario("1.26.0", "mAttachToEntity locator", Object{{"mAttachToEntity", a}});
        set.push_back(Object{{"mPresetIndex", 0}});
        set.push_back(Object{{"mPresetIndex", 1}, {"mPos", Object{{"mPos", xyz(-123.5, 70.25, 456.75)}}}, {"mRot", Object{{"mRotX", 45.0}, {"mRotY", -90.0}}}});
        set.push_back(Object{{"mPresetIndex", 2}, {"mFacing", Object{{"mFacingPos", xyz(0.0, 64.0, 0.0)}}}});
        set.push_back(Object{{"mPresetIndex", 3}, {"mViewOffset", Object{{"mViewOffsetX", 1.0}, {"mViewOffsetY", -0.5}}},
                             {"mEntityOffset", Object{{"mEntityOffsetX", 0.0}, {"mEntityOffsetY", 1.62}, {"mEntityOffsetZ", -2.0}}}});
        set.push_back(Object{{"mPresetIndex", 0}, {"mDefault", false}});
        set.push_back(Object{{"mPresetIndex", 0}, {"mDefault", true}, {"mRemoveIgnoreStartingValuesComponent", true}});
        p.values("mSet", set);
        p.values("mSpline", [&] {
            List l = spline;
            l.push_back(Object{{"mSplineIdentifier", "minecraft:orbit"}, {"mTotalTime", 10.0}, {"mCurveType", 0}, {"mLoadFromJson", true}});
            l.push_back(Object{{"mTotalTime", 1.0}, {"mCurveType", 1}, {"mLoadFromJson", false}});
            return l;
        }());
    }
}
