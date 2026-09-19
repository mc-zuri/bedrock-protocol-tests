#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field metadata(std::size_t at) { return vector("mCacheMetadata", at, 0x8, structure("value", 0, {u64("blobId", 0x0)})); }
Field chunk(std::size_t at) { return string("mSerializedChunk", at); }
Field pos(std::size_t at) { return structure("mPos", at, {i32("x", 0x0), i32("z", 0x4)}); }
}

BPX_PACKET("LevelChunkPacket") {
    p.shape("1.16.201", {boolean("mCacheEnabled", 0x0), pos(0x8), chunk(0x10), u32("mSubChunksCount", 0x30), metadata(0x38)});
    p.shape("1.17.30", {pos(0x0), boolean("mCacheEnabled", 0x8), chunk(0x10), u32("mSubChunksCount", 0x30), metadata(0x38)});
    p.shape("1.17.40", {pos(0x0), boolean("mCacheEnabled", 0x8), chunk(0x10), u32("mSubChunksCount", 0x30), metadata(0x40)});
    p.shape("1.18.11", {pos(0x0), boolean("mCacheEnabled", 0x8), chunk(0x10), u32("mSubChunksCount", 0x30),
                        boolean("mClientNeedsToRequestSubchunks", 0x39), i32("mClientRequestSubChunkLimit", 0x3C), metadata(0x40)});
    p.shape("1.20.61", {pos(0x0), i32("mDimensionId", 0x8), boolean("mCacheEnabled", 0xC), chunk(0x10), u32("mSubChunksCount", 0x30),
                        boolean("mClientNeedsToRequestSubchunks", 0x39), i32("mClientRequestSubChunkLimit", 0x3C), metadata(0x40)});
    p.shape("1.26.40", {pos(0x0), i32("mDimensionId", 0x8), u32("mSubChunksCount", 0xC),
                        optional("mClientRequestSubChunkLimit", 0x10, 0x4, i32("value", 0)), boolean("mCacheEnabled", 0x18),
                        metadata(0x20), chunk(0x38)});

    p.values("mPos", {Object{{"x", 0}, {"z", 0}}, Object{{"x", -1}, {"z", 1}}, Object{{"x", 1875000}, {"z", -1875000}}});
    p.values("mDimensionId", {0, 1, 2}); // Overworld, Nether, TheEnd
    p.values("mSubChunksCount", {0, 1, 4, 16, 24});
    p.values("mSerializedChunk", {"", std::string("\x00", 1), std::string("\x01\x02\x03\x00\xff", 5)});

    // the blob ids are only sent with the cache enabled
    List const blobs{List{Object{{"blobId", std::int64_t{0x123456789ABCDEFLL}}}},
                     List{Object{{"blobId", 1}}, Object{{"blobId", 2}}, Object{{"blobId", -1}}}};
    for (auto const& b : blobs) p.scenario("cache blobs", {{"mCacheEnabled", true}, {"mCacheMetadata", b}});
    p.scenario("cache enabled, no blobs", {{"mCacheEnabled", true}});
    // the client requests the sub-chunks: no limit (-1) or a u16 limit; the count is not sent then
    for (int limit : {-1, 0, 8, 65535})
        p.scenario("1.18.11", "1.26.40", "client requests sub-chunks",
                   {{"mClientNeedsToRequestSubchunks", true}, {"mClientRequestSubChunkLimit", limit}});
    for (int limit : {-1, 0, 8, 65535})
        p.scenario("1.26.40", "client requests sub-chunks", {{"mClientRequestSubChunkLimit", limit}});
}
