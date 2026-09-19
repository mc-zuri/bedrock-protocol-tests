#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PositionTrackingDBServerBroadcastPacket") {
    p.since("1.21.60");
    p.shape("1.21.60", {
        u8("action", 0x0),
        i32("id", 0x4),
    });

    p.values("action", {0, 1, 2}); // Action: Update, Destroy, NotFound
    p.values("id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
