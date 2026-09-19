#include "packets/Packet.h"

#include <cstdint>
#include <string>
#include <vector>

namespace {
using namespace bpx::schema;

struct Range {
    std::size_t inputData, words, inputMode, playMode, interaction, tick, blockActions, vehicle;
};
constexpr std::size_t kNone = ~std::size_t{0};

Layout layout(char era, Range r) {
    Layout l{vec2("mRot", 0x0), vec3("mPos", 0x8), f32("mYHeadRot", 0x14), vec3("mPosDelta", 0x18)};
    switch (era) {
    case 'a': l.push_back(vec2("mMove", 0x24)); l.push_back(vec3("mGazeDirection", 0x2C)); break;
    case 'b':
        l.push_back(vec2("mAnalogMoveVector", 0x24));
        l.push_back(vec2("mMove", 0x2C));
        l.push_back(vec3("mGazeDirection", 0x34));
        break;
    case 'c':
        l.push_back(vec2("mVehicleRot", 0x24));
        l.push_back(vec2("mAnalogMoveVector", 0x2C));
        l.push_back(vec2("mMove", 0x34));
        l.push_back(vec3("mGazeDirection", 0x3C));
        break;
    default: // 'd' 1.21.42, 'e' 1.21.50+
        l.push_back(vec2("mVehicleRot", 0x24));
        l.push_back(vec2("mAnalogMoveVector", 0x2C));
        l.push_back(vec2("mMove", 0x34));
        l.push_back(vec2("mInteractRotation", 0x3C));
        l.push_back(vec3("mCameraOrientation", 0x44));
        if (era == 'e') l.push_back(vec2("mRawMoveVector", 0x50));
    }
    std::vector<Field> words;
    if (r.words == 0) words.push_back(u32("w0", 0));
    for (std::size_t w = 0; w < r.words; ++w) words.push_back(u64("w" + std::to_string(w), w * 8));
    l.push_back(structure("mInputData", r.inputData, words));
    l.push_back(u32("mInputMode", r.inputMode));
    l.push_back(u32("mPlayMode", r.playMode));
    if (r.interaction != kNone) l.push_back(u32("mNewInteractionModel", r.interaction));
    l.push_back(u64("mClientTick", r.tick));
    if (r.blockActions != kNone)
        l.push_back(vector("mPlayerBlockActions", r.blockActions, 0x14,
                           structure("value", 0, {i32("mPlayerActionType", 0x0), blockPos("mPos", 0x4), i32("mFacing", 0x10)})));
    if (r.vehicle != kNone) l.push_back(i64("mClientPredictedVehicle", r.vehicle));
    return l;
}

// never the item bits (34, 36): they need the item data we leave null; from 1.21.111 the reader also rejects the
// deprecated NorthJump (2), IsCameraRelativeMovementEnabled (54) and IsRotControlledByMoveDirection (55)
List inputBits(int count, int words, bool deprecated = true) {
    auto bad = [&](int b) { return b == 34 || b == 36 || (!deprecated && (b == 2 || b == 54 || b == 55)); };
    int const n = words == 0 ? 1 : words;
    auto obj = [&](std::vector<std::uint64_t> const& v) {
        Object o;
        for (int w = 0; w < n; ++w) o.emplace_back("w" + std::to_string(w), static_cast<std::int64_t>(v[w]));
        return o;
    };
    List out;
    std::vector<std::uint64_t> all(n, 0);
    for (int b = 0; b < count; ++b) {
        if (bad(b)) continue;
        std::vector<std::uint64_t> one(n, 0);
        one[b / 64] = 1ULL << (b % 64);
        all[b / 64] |= 1ULL << (b % 64);
        out.push_back(obj(one));
    }
    out.push_back(obj(std::vector<std::uint64_t>(n, 0)));
    out.push_back(obj(all));
    return out;
}

// only the break actions carry a position and face, until 1.26.40 where cereal writes all three members
List blockActions(int count, bool allCarryPosition) {
    List out;
    for (int a = 0; a < count; ++a) {
        bool const pos = allCarryPosition || a == 0 || a == 1 || a == 18 || a == 26 || a == 27;
        Object xyz{{"x", pos ? -123 : 0}, {"y", pos ? -60 : 0}, {"z", pos ? 456 : 0}};
        out.push_back(Object{{"mPlayerActionType", a}, {"mPos", xyz}, {"mFacing", pos ? a % 6 : 0}});
    }
    return out;
}

Object bit(int b) { return Object{{b < 64 ? "w0" : "w1", static_cast<std::int64_t>(1ULL << (b % 64))}}; }
}

BPX_PACKET("PlayerAuthInputPacket") {
    p.shape("1.16.201", layout('a', {0x38, 0, 0x3C, 0x40, kNone, 0x48, kNone, kNone}));
    p.shape("1.16.210", layout('a', {0x38, 1, 0x40, 0x44, kNone, 0x48, 0x60, kNone}));
    p.shape("1.19.1", layout('a', {0x38, 1, 0x40, 0x44, 0x48, 0x50, 0x68, kNone}));
    p.shape("1.19.70", layout('b', {0x40, 1, 0x48, 0x4C, 0x50, 0x58, 0x70, kNone}));
    p.shape("1.20.61", layout('b', {0x40, 1, 0x48, 0x4C, 0x50, 0x58, 0x70, 0x88}));
    p.shape("1.20.71", layout('c', {0x48, 1, 0x50, 0x54, 0x58, 0x60, 0x78, 0x90}));
    p.shape("1.21.42", layout('d', {0x50, 1, 0x58, 0x5C, 0x60, 0x68, 0x80, 0x98}));
    p.shape("1.21.50", layout('e', {0x58, 2, 0x68, 0x6C, 0x70, 0x78, 0x90, 0xA8}));

    p.values("mRot", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 45.5}, {"y", -90.0}}, Object{{"x", -89.9}, {"y", 179.9}}});
    p.values("mPos", {Object{{"x", 0.5}, {"y", 65.62}, {"z", 0.5}}, Object{{"x", -123.25}, {"y", -60.0}, {"z", 456.75}},
                      Object{{"x", 29999999.5}, {"y", 320.0}, {"z", -29999999.5}}});
    p.values("mYHeadRot", {0.0, 90.0, -179.9});
    p.values("mPosDelta", {Object{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}, Object{{"x", 0.1}, {"y", -0.0784}, {"z", -0.2}}});
    List const moves{Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 0.0}, {"y", 1.0}}, Object{{"x", -0.7071}, {"y", 0.7071}}};
    p.values("mMove", moves);
    p.values("mAnalogMoveVector", moves);
    p.values("mRawMoveVector", moves);
    p.values("mInteractRotation", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 30.0}, {"y", -135.0}}});
    p.values("mCameraOrientation", {Object{{"x", 0.0}, {"y", 0.0}, {"z", 1.0}}, Object{{"x", 0.5}, {"y", -0.5}, {"z", 0.7071}}});
    p.values("mClientTick", {0, 1, std::int64_t{123456789}});

    struct Bits {
        char const* since;
        int         count;
    };
    for (Bits b : {Bits{"1.16.201", 25}, Bits{"1.16.210", 37}, Bits{"1.19.60", 38}, Bits{"1.19.70", 39}, Bits{"1.20.10", 42},
                   Bits{"1.20.30", 44}, Bits{"1.20.40", 45}, Bits{"1.20.61", 46}, Bits{"1.20.71", 48}, Bits{"1.21.0", 49},
                   Bits{"1.21.30", 53}, Bits{"1.21.42", 57}})
        p.values(b.since, "mInputData", inputBits(b.count, std::string(b.since) == "1.16.201" ? 0 : 1));
    p.values("1.21.50", "mInputData", inputBits(65, 2));
    p.values("1.21.111", "mInputData", inputBits(65, 2, false));
    p.values("1.26.40", "mInputData", inputBits(66, 2, false)); // + InternalUpdate
    // InputMode: Undefined, Mouse, Touch, GamePad, MotionController; ClientPlayMode: Normal, Teaser, Screen, Viewer,
    // Reality, Placement, LivingRoom, ExitLevel, ExitLevelLivingRoom. From 1.26.40 the reader rejects the deprecated ones.
    p.values("mInputMode", {0, 1, 2, 3, 4});
    p.values("1.26.40", "mInputMode", {0, 1, 2, 3});
    p.values("mPlayMode", {0, 1, 2, 3, 4, 5, 6, 7, 8});
    p.values("1.26.40", "mPlayMode", {0, 1, 2, 7});
    p.values("mNewInteractionModel", {0, 1, 2});         // Touch, Crosshair, Classic

    // sent only when switched on: gaze by Reality mode (4), vehicle by IsInClientPredictedVehicle (45), block actions by
    // PerformBlockActions (35)
    p.scenario("gaze direction in Reality mode",
               {{"mPlayMode", 4}, {"mGazeDirection", Object{{"x", 0.0}, {"y", -0.5}, {"z", 0.866}}}});
    p.scenario("1.20.61", "client predicted vehicle", {{"mInputData", bit(45)}, {"mClientPredictedVehicle", std::int64_t{-4294967295LL}}});
    p.scenario("1.20.71", "client predicted vehicle rotation",
               {{"mInputData", bit(45)}, {"mVehicleRot", Object{{"x", 10.0}, {"y", -170.0}}}, {"mClientPredictedVehicle", 12}});
    struct Actions {
        char const* since;
        int         count;
    };
    for (Actions a : {Actions{"1.16.210", 28}, Actions{"1.19.1", 30}, Actions{"1.19.60", 31}, Actions{"1.20.10", 34},
                      Actions{"1.20.30", 36}, Actions{"1.20.40", 37}, Actions{"1.21.50", 38}})
        p.scenario(a.since, "1.26.40", std::string("block actions 0..") + std::to_string(a.count - 1),
                   {{"mInputData", bit(35)}, {"mPlayerBlockActions", blockActions(a.count, false)}});
    // 1.26.40+: the reader rejects every action but the block-breaking ones (probed): StartDestroyBlock,
    // AbortDestroyBlock, StopDestroyBlock, CrackBlock, PredictDestroyBlock, ContinueDestroyBlock
    List breaking;
    for (int a : {0, 1, 2, 18, 26, 27}) breaking.push_back(blockActions(39, true)[a]);
    p.scenario("1.26.40", "block-breaking actions", {{"mInputData", bit(35)}, {"mPlayerBlockActions", breaking}});
}
