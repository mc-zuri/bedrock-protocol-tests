#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("EmoteListPacket") {
    p.shape("1.16.201", {
        i64("runtime_id", 0x0),
    });

    p.values("runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
