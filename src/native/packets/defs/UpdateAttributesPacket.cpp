#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("UpdateAttributesPacket") {
    p.shape("1.16.201", {
        i64("target_runtime_id", 0x0),
        i64("tick", 0x20),
    });

    p.values("target_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("tick", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
