#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PositionTrackingDBServerBroadcast") {
    p.until("1.21.60");
    p.shape("1.16.201", {
        u8("broadcast_action", 0x0),
        i32("tracking_id", 0x4),
    });

    p.values("broadcast_action", {0, 1, 2, 255});
    p.values("tracking_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
