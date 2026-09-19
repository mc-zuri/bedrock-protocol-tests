#pragma once
// mType bits: TextureUpdate 2, DecorationUpdate 4, Creation 8. Decorations and tracked actors are left empty.

#include "packets/Packet.h"

#include <cstdint>
#include <string>

namespace bpx::packets::mapitem {

using namespace bpx::schema;

inline Layout layoutA() {
    return {vector("mMapIds", 0x0, 8, i64("value", 0)), i8("mScale", 0x18), i32("mStartX", 0x50), i32("mStartY", 0x54),
            u8("mDimension", 0x58), i32("mWidth", 0x5C), i32("mHeight", 0x60), i32("mType", 0x64),
            vector("mMapPixels", 0x68, 4, u32("value", 0)), boolean("mLocked", 0x80)};
}
inline Layout layoutB() {
    return {vector("mMapIds", 0x0, 8, i64("value", 0)), i8("mScale", 0x18), i32("mStartX", 0x50), i32("mStartY", 0x54),
            blockPos("mMapOrigin", 0x58), u8("mDimension", 0x64), i32("mWidth", 0x68), i32("mHeight", 0x6C), i32("mType", 0x70),
            vector("mMapPixels", 0x78, 4, u32("value", 0)), boolean("mLocked", 0x90)};
}
inline Layout layoutC() {
    return {i64("mMapId", 0x0), i32("mType", 0x8), u8("mDimension", 0xC), boolean("mLocked", 0xD), blockPos("mMapOrigin", 0x10),
            vector("mCreationMapIds", 0x20, 8, i64("value", 0)), i8("mScale", 0x38), i32("mWidth", 0x70), i32("mHeight", 0x74),
            i32("mStartX", 0x78), i32("mStartY", 0x7C), vector("mMapPixels", 0x80, 4, u32("value", 0))};
}

// Before 1.26.40 the map's own id is the first of mMapIds.
inline void values(PacketDef& p) {
    std::int64_t const map = -4294967295LL;
    p.baseline("mMapIds", List{map});
    p.baseline("mMapId", map);
    p.values("mMapIds", {List{map}, List{std::int64_t{1}}});
    p.values("mMapId", {map, 1});
    p.values("mDimension", {0, 1, 2}); // Overworld, Nether, TheEnd
    p.values("mLocked", {false, true});
    p.values("mMapOrigin", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -1024}, {"y", -64}, {"z", 2048}}});
    p.values("mType", {0});

    List pixels;
    for (std::uint32_t c : {0xFF000000u, 0xFFFFFFFFu, 0xFF3F76E4u, 0x00000000u}) pixels.push_back(std::int64_t{c});
    Object texture{{"mType", 2}, {"mScale", 0}, {"mWidth", 2}, {"mHeight", 2}, {"mStartX", 0}, {"mStartY", 0}, {"mMapPixels", pixels}};
    Object textureAt{{"mType", 2}, {"mScale", 4}, {"mWidth", 1}, {"mHeight", 4}, {"mStartX", 64}, {"mStartY", 12}, {"mMapPixels", pixels}};
    for (int scale = 0; scale <= 4; ++scale)
        p.scenario("decorations scale " + std::to_string(scale), Object{{"mType", 4}, {"mScale", scale}});
    p.scenario("texture", texture);
    p.scenario("texture offset", textureAt);
    p.scenario("creation", Object{{"mType", 8}, {"mScale", 0}, {"mMapIds", List{map, std::int64_t{1}, std::int64_t{2}}}});
    p.scenario("creation (1.26.40)", Object{{"mType", 8}, {"mScale", 0}, {"mCreationMapIds", List{map, std::int64_t{1}}}});
    Object all = texture;
    all[0].second = 14;
    p.scenario("all updates", all);
}

}
