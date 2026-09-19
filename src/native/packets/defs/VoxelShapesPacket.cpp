#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("VoxelShapesPacket") {
    p.since("1.26.0");
    p.shape("1.26.0", {
        i32("size_t", 0x28),
        i32("name_map", 0x58),
    });
    p.shape("1.26.10", {
        i32("size_t", 0x28),
        i16("custom_shape_count", 0x58),
    });

    p.values("size_t", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("name_map", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("custom_shape_count", {0, 1, -1, 32767, -32768});

    // narrowed to what the game reads back unchanged
    p.values("name_map", {5});
    p.values("size_t", {0});
}
