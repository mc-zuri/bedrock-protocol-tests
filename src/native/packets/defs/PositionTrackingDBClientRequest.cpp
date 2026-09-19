#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PositionTrackingDBClientRequest") {
    p.until("1.20.80");
    p.shape("1.16.201", {
        u8("request_action", 0x0),
        i32("tracking_id", 0x4),
    });

    p.values("request_action", {0, 1, 2, 255});
    p.values("tracking_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
