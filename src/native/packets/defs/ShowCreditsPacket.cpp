#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ShowCreditsPacket") {
    p.shape("1.16.201", {
        i64("player_runtime_id", 0x0),
        i32("credits_state", 0x8),
    });

    p.values("player_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("credits_state", {0, 1}); // Start, Finished
}
