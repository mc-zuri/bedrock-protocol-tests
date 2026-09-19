#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RequestChunkRadiusPacket") {
    p.shape("1.16.201", {
        i32("chunk_radius", 0x0),
    });
    p.shape("1.19.80", {
        i64("chunk_radius", 0x0),
    });
    p.shape("1.21.60", {
        i32("chunk_radius", 0x0),
        u8("max_chunkradius", 0x4),
    });

    p.values("chunk_radius", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("max_chunkradius", {0, 1, 2, 255});
}
