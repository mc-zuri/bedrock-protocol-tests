#include "packets/Packet.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace {
using namespace bpx::schema;

std::vector<char const*> const kAttributes = {"mMovementSpeed", "mUnderwaterMovementSpeed", "mLavaMovementSpeed", "mJumpStrength",
                                              "mHealth",        "mHunger",                  "mFrictionModifier", "mBounciness",
                                              "mAirDragModifier"};

Layout layout(int words, int attributes, bool flying) {
    std::vector<Field> bits;
    for (int w = 0; w < words; ++w) bits.push_back(u64("w" + std::to_string(w), w * 8));
    std::size_t const box = words * 8;
    Layout l{structure("mActorDataFlag", 0x0, bits),
             structure("mActorBoundingBox", box, {f32("mScale", 0x0), f32("mWidth", 0x4), f32("mHeight", 0x8)})};
    std::vector<Field> attrs;
    for (int a = 0; a < attributes; ++a) attrs.push_back(optional(kAttributes[a], a * 8, 4, f32("value", 0)));
    std::size_t const attrAt = box + 0xC;
    l.push_back(structure("mMovementAttributes", attrAt, attrs));
    std::size_t const id = (attrAt + attributes * 8 + 7) / 8 * 8;
    l.push_back(i64("mActorID", id));
    if (flying) l.push_back(boolean("mIsFlying", id + 8));
    return l;
}

List flagBits(int count, int words) {
    List out;
    auto word = [&](int bit) {
        Object o;
        for (int w = 0; w < words; ++w) o.emplace_back("w" + std::to_string(w), std::int64_t{w == bit / 64 ? std::int64_t(1ULL << (bit % 64)) : 0});
        return o;
    };
    for (int b = 0; b < count; ++b) out.push_back(word(b));
    Object none, all;
    for (int w = 0; w < words; ++w) {
        int const in = count - w * 64;
        none.emplace_back("w" + std::to_string(w), 0);
        all.emplace_back("w" + std::to_string(w),
                         std::int64_t(in >= 64 ? ~0ULL : in > 0 ? (1ULL << in) - 1 : 0));
    }
    out.push_back(none);
    out.push_back(all);
    return out;
}
}

BPX_PACKET("ClientMovementPredictionSyncPacket") {
    p.since("1.21.60");
    p.shape("1.21.60", layout(2, 6, false));
    p.shape("1.21.70", layout(2, 6, true));
    p.shape("1.26.20", layout(3, 9, true));

    // ActorFlags::Count per version
    struct Count {
        char const* since;
        int         count;
    };
    for (Count c : {Count{"1.21.60", 120}, Count{"1.21.70", 123}, Count{"1.21.80", 124}, Count{"1.21.90", 125},
                    Count{"1.21.111", 126}, Count{"1.21.130", 127}}) p.values(c.since, "mActorDataFlag", flagBits(c.count, 2));
    p.values("1.26.20", "mActorDataFlag", flagBits(130, 3));
    p.values("1.26.40", "mActorDataFlag", flagBits(131, 3));

    p.values("mActorBoundingBox", {Object{{"mScale", 1.0}, {"mWidth", 0.6}, {"mHeight", 1.8}},
                                   Object{{"mScale", 0.5}, {"mWidth", 0.3}, {"mHeight", 0.9}}});
    List attrs{Object{}};
    for (auto const* a : kAttributes) attrs.push_back(Object{{a, 0.1}});
    attrs.push_back(Object{{"mMovementSpeed", 0.1}, {"mUnderwaterMovementSpeed", 0.02}, {"mLavaMovementSpeed", 0.02},
                           {"mJumpStrength", 0.42}, {"mHealth", 20.0}, {"mHunger", 20.0}});
    p.values("mMovementAttributes", attrs);
    p.values("mActorID", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mIsFlying", {false, true});
}
