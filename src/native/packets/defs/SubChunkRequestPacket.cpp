#include "packets/Packet.h"

namespace {
using namespace bpx::schema;
Object pos(int x, int y, int z) { return Object{{"x", x}, {"y", y}, {"z", z}}; }
Object offset(int x, int y, int z) { return Object{{"mX", x}, {"mY", y}, {"mZ", z}}; }
}

BPX_PACKET("SubChunkRequestPacket") {
    p.since("1.17.40");
    p.shape("1.17.40", {i32("mDimensionType", 0x0), structure("mSubChunkPos", 0x4, {i32("x", 0), i32("y", 4), i32("z", 8)})});
    auto layout = [](std::size_t offsets, std::size_t center, bool count) {
        Layout l{i32("mDimensionType", 0x0),
                 vector("mSubChunkPosOffsets", offsets, 3, structure("value", 0, {i8("mX", 0), i8("mY", 1), i8("mZ", 2)})),
                 structure("mCenterPos", center, {i32("x", 0), i32("y", 4), i32("z", 8)})};
        if (count) l.push_back(u32("mRequestCount", 0x38));
        return l;
    };
    p.shape("1.18.11", layout(0x20, 0x3C, true));
    p.shape("1.26.30", layout(0x8, 0x20, false));

    p.values("mDimensionType", {0, 1, 2});
    p.values("mSubChunkPos", {pos(0, 4, 0), pos(-100, -4, 3000)});
    p.values("mCenterPos", {pos(0, 4, 0), pos(-100, -4, 3000)});
    List const one{offset(0, 0, 0)}, three{offset(-1, -1, -1), offset(1, 2, 1), offset(-128, 127, 0)};
    p.values("1.26.30", "mSubChunkPosOffsets", {List{}, one, three});
    // the writer sends mRequestCount offsets, so the count is set with the list
    p.scenario("1.18.11", "1.26.30", "one offset", {{"mSubChunkPosOffsets", one}, {"mRequestCount", 1}});
    p.scenario("1.18.11", "1.26.30", "three offsets", {{"mSubChunkPosOffsets", three}, {"mRequestCount", 3}});
}
