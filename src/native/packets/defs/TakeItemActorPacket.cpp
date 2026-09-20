#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("TakeItemActorPacket") {
    p.shape("1.16.201", {
        i64("item_runtime_id", 0x0),
        i64("actor_runtime_id", 0x8),
    });

    p.values("item_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    // the ids a server hands out are small; the protocol reads this one as a 32-bit varint
    p.values("actor_runtime_id", {0, 1, 12345, INT_MAX});
}
