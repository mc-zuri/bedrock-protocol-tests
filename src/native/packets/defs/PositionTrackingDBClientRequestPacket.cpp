#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PositionTrackingDBClientRequestPacket") {
    p.since("1.20.80");
    p.shape("1.20.80", {
        u8("action", 0x0),
        i32("id", 0x4),
    });

    p.values("action", {0}); // Action: Query
    p.values("id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
