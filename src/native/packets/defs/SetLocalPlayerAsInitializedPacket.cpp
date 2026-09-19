#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetLocalPlayerAsInitializedPacket") {
    p.shape("1.16.201", {
        i64("player_id", 0x0),
    });

    p.values("player_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
