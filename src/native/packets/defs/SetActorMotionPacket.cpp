#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetActorMotionPacket") {
    p.shape("1.16.201", {
        i64("target_runtime_id", 0x0),
        f32("motion_x", 0x8),
        f32("motion_y", 0xC),
        f32("motion_z", 0x10),
    });
    p.shape("1.20.71", {
        i64("target_runtime_id", 0x0),
        f32("motion_x", 0x8),
        f32("motion_y", 0xC),
        f32("motion_z", 0x10),
        i64("tick", 0x18),
    });

    p.values("target_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("motion_x", {0.0, 1.5, -1.0, 1e6});
    p.values("motion_y", {0.0, 1.5, -1.0, 1e6});
    p.values("motion_z", {0.0, 1.5, -1.0, 1e6});
    p.values("tick", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
