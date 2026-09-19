#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RemoveActorPacket") {
    p.shape("1.16.201", {
        i64("target_actor_id", 0x0),
    });
    p.shape("1.21.111", {});
    p.shape("1.21.130", {
        i64("target_actor_id", 0x0),
    });

    p.values("target_actor_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
