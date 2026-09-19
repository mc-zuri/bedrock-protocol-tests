// SubChunkRequestResult: Undefined, Success, LevelChunkDoesntExist, WrongDimension, PlayerDoesntExist,
// IndexOutOfBounds, SuccessAllAir. HeightMapDataType: NoData, HasData, AllTooHigh, AllTooLow, AllCopied.
// On the wire (to 1.26.30) the data is skipped for cached all-air sub-chunks, the height map is sent only with
// HasData and the blob id only with the cache on; the entries below are built to match.
#include "packets/Packet.h"

#include <cstdint>
#include <string>
#include <vector>

namespace {
using namespace bpx::schema;

// schar[16][16] as 32 little-endian words
Field heightMap(char const* name, std::size_t at) {
    std::vector<Field> w;
    for (int i = 0; i < 32; ++i) w.push_back(u64("w" + std::to_string(i), i * 8));
    return structure(name, at, w);
}
Object heights(std::int8_t base) {
    Object o;
    for (int i = 0; i < 32; ++i) {
        std::uint64_t v = 0;
        for (int b = 0; b < 8; ++b) v |= std::uint64_t(std::uint8_t(base + (i * 8 + b) % 16)) << (b * 8);
        o.emplace_back("w" + std::to_string(i), static_cast<std::int64_t>(v));
    }
    return o;
}
Field offset() { return structure("mSubChunkPosOffset", 0x0, {i8("mX", 0), i8("mY", 1), i8("mZ", 2)}); }

enum Era { Plain, Render, Optionals };

Field entry(Era era) {
    if (era == Optionals)
        return structure("value", 0,
                         {offset(), u8("mResult", 0x3), optional("mSerializedSubChunk", 0x8, 0x20, string("value", 0)),
                          u8("mHeightMapType", 0x30), optional("mSubchunkHeightMap", 0x31, 0x100, heightMap("value", 0)),
                          u8("mRenderHeightMapType", 0x132), optional("mSubchunkRenderHeightMap", 0x133, 0x100, heightMap("value", 0)),
                          optional("mBlobId", 0x238, 8, u64("value", 0))});
    std::vector<Field> m{offset(), string("mSerializedSubChunk", 0x8), u8("mResult", 0x28), u8("mHeightMapType", 0x29),
                         heightMap("mSubchunkHeightMap", 0x2A)};
    if (era == Render) {
        m.push_back(u8("mRenderHeightMapType", 0x12A));
        m.push_back(heightMap("mSubchunkRenderHeightMap", 0x12B));
    }
    m.push_back(u64("mBlobId", era == Render ? 0x230 : 0x130));
    return structure("value", 0, m);
}

Layout packet(Era era) {
    std::size_t const size = era == Plain ? 0x138 : era == Render ? 0x238 : 0x248;
    return {boolean("mCacheEnabled", 0x0), i32("mDimensionType", 0x4), vector("mSubChunkData", 0x8, size, entry(era)),
            structure("mCenterPos", 0x20, {i32("x", 0), i32("y", 4), i32("z", 8)})};
}

Object pos(std::int8_t x, std::int8_t y, std::int8_t z) { return Object{{"mX", x}, {"mY", y}, {"mZ", z}}; }
std::string const kData("\x09\x01\x00\x00", 4); // a sub-chunk (version 9, one layer), truncated: sent as is
}

BPX_PACKET("SubChunkPacket") {
    p.since("1.17.40");
    Layout single{i32("mDimensionType", 0x0), structure("mSubChunkPos", 0x4, {i32("x", 0), i32("y", 4), i32("z", 8)}),
                  string("mSerializedSubChunk", 0x10), i32("mResult", 0x30), u8("mHeightMapType", 0x34),
                  heightMap("mSubchunkHeightMap", 0x35)};
    p.shape("1.17.40", single);
    single.push_back(boolean("mCacheEnabled", 0x135));
    single.push_back(u64("mBlobId", 0x138));
    p.shape("1.18.0", single);
    p.shape("1.18.11", packet(Plain));
    p.shape("1.21.90", packet(Render));
    p.shape("1.26.40", packet(Optionals));

    p.values("mDimensionType", {0, 1, 2});
    p.values("mCenterPos", {Object{{"x", 0}, {"y", 4}, {"z", 0}}, Object{{"x", -100}, {"y", -4}, {"z", 3000}}});
    p.values("mSubChunkPos", {Object{{"x", 0}, {"y", 4}, {"z", 0}}, Object{{"x", -100}, {"y", -4}, {"z", 3000}}});
    p.values("mResult", {0, 1, 2, 3, 4, 5, 6});
    p.values("mSerializedSubChunk", {"", kData});
    p.values("mHeightMapType", {0, 2, 3, 4});
    p.scenario("height map", {{"mHeightMapType", 1}, {"mSubchunkHeightMap", heights(-3)}});
    p.scenario("1.18.0", "cached", {{"mCacheEnabled", true}, {"mResult", 1}, {"mBlobId", std::int64_t{0x0123456789ABCDEFLL}}});

    struct Entry {
        Object      offset;
        int         result;
        std::string data;
        int         type, renderType;
    };
    std::vector<Entry> const entries{{pos(0, 0, 0), 1, kData, 0, 0}, {pos(-1, 2, 1), 1, kData, 2, 3}, {pos(1, -2, -1), 6, "", 3, 4},
                                     {pos(0, 1, 0), 2, "", 4, 0},     {pos(0, 0, 1), 3, "", 0, 0},    {pos(0, 0, 0), 4, "", 0, 0},
                                     {pos(0, 0, 0), 5, "", 0, 0},     {pos(0, 0, 0), 0, "", 0, 0}};
    auto build = [&](bool render, bool undefined = true) {
        List all;
        for (auto const& e : entries) {
            if (e.result == 0 && !undefined) continue;
            // 1.21.90+: AllCopied is only accepted for the render height map (the reader rejects it for the other)
            Object o{{"mSubChunkPosOffset", e.offset}, {"mResult", e.result}, {"mHeightMapType", render && e.type == 4 ? 3 : e.type}};
            o.emplace_back("mSerializedSubChunk", e.data); // always sent without the cache (1.26.40+: engaged)
            if (render) o.emplace_back("mRenderHeightMapType", e.renderType);
            all.push_back(o);
        }
        Object hm{{"mSubChunkPosOffset", pos(0, 0, 0)}, {"mResult", 1}, {"mSerializedSubChunk", kData}, {"mHeightMapType", 1},
                  {"mSubchunkHeightMap", heights(-3)}};
        if (render) {
            hm.emplace_back("mRenderHeightMapType", 1);
            hm.emplace_back("mSubchunkRenderHeightMap", heights(5));
        }
        all.push_back(hm);
        return all;
    };
    p.values("1.18.11", "mSubChunkData", {List{}, build(false)});
    p.values("1.21.90", "mSubChunkData", {List{}, build(true)});
    p.values("1.26.40", "mSubChunkData", {List{}, build(true, false)}); // the reader rejects Undefined results
    // with the cache on: a blob id per entry, and no data for all-air sub-chunks
    Object cachedAir{{"mSubChunkPosOffset", pos(0, 1, 0)}, {"mResult", 6}, {"mHeightMapType", 3}, {"mBlobId", 42}};
    Object cached{{"mSubChunkPosOffset", pos(0, 0, 0)}, {"mResult", 1}, {"mSerializedSubChunk", kData}, {"mHeightMapType", 0},
                  {"mBlobId", std::int64_t{0x0123456789ABCDEFLL}}};
    p.scenario("1.18.11", "cached entries", {{"mCacheEnabled", true}, {"mSubChunkData", List{cached, cachedAir}}});
}
