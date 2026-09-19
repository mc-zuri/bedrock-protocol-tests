#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("MoveActorAbsolutePacket") {
    p.shape("1.16.201", {
        i64("move_data_actorruntimeid", 0x0),
        u8("move_data_header", 0x8),
        f32("move_data_position_x", 0xC),
        f32("move_data_position_y", 0x10),
        f32("move_data_position_z", 0x14),
        i32("rotation_x", 0x18),
    });
    p.shape("1.21.60", {
        i64("move_data_actorruntimeid", 0x0),
        u8("move_data_header", 0x8),
        f32("move_data_position_x", 0xC),
        f32("move_data_position_y", 0x10),
        f32("move_data_position_z", 0x14),
        u8("move_data_rotation_x", 0x18),
        u8("move_data_rotation_y", 0x19),
        u8("move_data_rotation_y_head", 0x1A),
    });

    p.values("move_data_actorruntimeid", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("move_data_header", {0, 1, 2, 255});
    p.values("move_data_position_x", {0.0, 1.5, -1.0, 1e6});
    p.values("move_data_position_y", {0.0, 1.5, -1.0, 1e6});
    p.values("move_data_position_z", {0.0, 1.5, -1.0, 1e6});
    p.values("rotation_x", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("move_data_rotation_x", {0, 1, 2, 255});
    p.values("move_data_rotation_y", {0, 1, 2, 255});
    p.values("move_data_rotation_y_head", {0, 1, 2, 255});
}
