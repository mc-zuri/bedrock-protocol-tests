#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("NetworkStackLatencyPacket") {
    p.shape("1.16.201", {
        i64("creation_time", 0x0),
        boolean("is_from_server", 0x8),
    });

    p.values("creation_time", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("is_from_server", {false, true});

    // what the game reads back unchanged
    p.values("creation_time", {0});
    p.baseline("creation_time", 0);  // default 7355145495100 is not one the game takes
    p.values("is_from_server", {});  // nothing the game reads back unchanged
}
