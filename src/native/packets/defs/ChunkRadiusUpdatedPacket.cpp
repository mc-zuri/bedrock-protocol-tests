#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ChunkRadiusUpdatedPacket") {
    p.shape("1.16.201", {
        i32("chunk_radius", 0x0),
    });

    p.values("chunk_radius", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
