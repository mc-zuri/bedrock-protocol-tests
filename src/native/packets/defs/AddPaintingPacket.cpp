#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AddPaintingPacket") {
    p.shape("1.16.201", {
        i64("target_actor_id", 0x0),
        i64("target_runtime_id", 0x8),
        f32("position_x", 0x10),
        f32("position_y", 0x14),
        f32("position_z", 0x18),
        i32("direction", 0x1C),
        string("motif", 0x20),
    });

    p.values("target_actor_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("target_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("position_x", {0.0, 1.5, -1.0, 1e6});
    p.values("position_y", {0.0, 1.5, -1.0, 1e6});
    p.values("position_z", {0.0, 1.5, -1.0, 1e6});
    p.values("direction", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("motif", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
