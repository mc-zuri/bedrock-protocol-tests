#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CameraPacket") {
    p.shape("1.16.201", {
        i64("camera_id", 0x0),
        i64("target_player_id", 0x8),
    });

    p.values("camera_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("target_player_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
