#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("MapCreateLockedCopyPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {
        i64("original_map_id", 0x0),
        i64("new_map_id", 0x8),
    });

    p.values("original_map_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("new_map_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
