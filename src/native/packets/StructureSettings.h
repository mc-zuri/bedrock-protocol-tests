#pragma once
// Before 1.20.40 the game serializes a copy of the settings, so these offsets do not apply.

#include "packets/Packet.h"

#include <cstdint>

namespace bpx::packets::structure {

using namespace bpx::schema;

inline void settingsA(Layout& l, std::size_t at) {
    for (Field f : {string("mPaletteName", at), boolean("mIgnoreEntities", at + 0x20), boolean("mIgnoreBlocks", at + 0x22),
                    boolean("mAllowNonTickingChunks", at + 0x25), blockPos("mStructureSize", at + 0x28),
                    blockPos("mStructureOffset", at + 0x34), vec3("mPivot", at + 0x40), i64("mLastTouchedByPlayer", at + 0x50),
                    u8("mRotation", at + 0x58), u8("mMirror", at + 0x59), u8("mAnimationMode", at + 0x5A),
                    f32("mAnimationSeconds", at + 0x5C), f32("mIntegrityValue", at + 0x60), u32("mIntegritySeed", at + 0x64)})
        l.push_back(std::move(f));
}

inline void settingsB(Layout& l, std::size_t at) {
    for (Field f : {string("mPaletteName", at), boolean("mIgnoreEntities", at + 0x20), boolean("mReloadActorEquipment", at + 0x21),
                    boolean("mIgnoreBlocks", at + 0x22), boolean("mIsWaterLogged", at + 0x23), boolean("mIgnoreJigsawBlocks", at + 0x24),
                    boolean("mAllowNonTickingChunks", at + 0x25), i64("mLastTouchedByPlayer", at + 0x28),
                    blockPos("mStructureSize", at + 0x30), blockPos("mStructureOffset", at + 0x3C), vec3("mPivot", at + 0x48),
                    u8("mMirror", at + 0x54), u8("mRotation", at + 0x55), u8("mAnimationMode", at + 0x56),
                    f32("mAnimationSeconds", at + 0x58), f32("mIntegrityValue", at + 0x5C), u32("mIntegritySeed", at + 0x60)})
        l.push_back(std::move(f));
}

inline void values(PacketDef& p) {
    p.values("mPaletteName", {"", "default"});
    for (char const* f : {"mIgnoreEntities", "mReloadActorEquipment", "mIgnoreBlocks", "mIsWaterLogged", "mIgnoreJigsawBlocks",
                          "mAllowNonTickingChunks"})
        p.values(f, {false, true});
    p.values("mLastTouchedByPlayer", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mStructureSize", {Object{{"x", 1}, {"y", 1}, {"z", 1}}, Object{{"x", 64}, {"y", 32}, {"z", 16}}});
    p.values("mStructureOffset", {Object{{"x", 0}, {"y", 1}, {"z", 0}}, Object{{"x", -5}, {"y", -2}, {"z", 7}}});
    p.values("mPivot", {Object{{"x", 0.0}, {"y", 0.0}, {"z", 0.0}}, Object{{"x", 8.5}, {"y", 4.0}, {"z", 0.5}}});
    p.values("mRotation", {0, 1, 2, 3});      // None, Rotate90, Rotate180, Rotate270
    p.values("mMirror", {0, 1, 2, 3});        // None, X, Z, Xz
    p.values("mAnimationMode", {0, 1, 2});    // None, Layers, Blocks
    p.values("mAnimationSeconds", {0.0, 1.5, 10.0});
    p.values("mIntegrityValue", {0.0, 0.5, 1.0});
    p.values("mIntegritySeed", {0, 1, 123456});
}

}
