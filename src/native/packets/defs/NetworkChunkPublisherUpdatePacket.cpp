#include "packets/Packet.h"

namespace {
using namespace bpx::schema;
Object chunk(int x, int z) { return Object{{"x", x}, {"z", z}}; }
}

BPX_PACKET("NetworkChunkPublisherUpdatePacket") {
    p.shape("1.16.201", {blockPos("mPosition", 0x0), u32("mRadius", 0xC)});
    p.shape("1.19.20", {blockPos("mPosition", 0x0), u32("mRadius", 0xC),
                        vector("mServerBuiltChunks", 0x10, 8, structure("value", 0, {i32("x", 0x0), i32("z", 0x4)}))});

    p.values("mPosition", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                           Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mRadius", {0, 16, 128, 512});
    p.values("mServerBuiltChunks", {List{}, List{chunk(0, 0)}, List{chunk(-8, 27), chunk(1875, -1875), chunk(0, 1)}});
}
