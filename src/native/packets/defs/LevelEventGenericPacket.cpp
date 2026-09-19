#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("LevelEventGenericPacket") {
    p.shape("1.16.201", {
        i32("event_id", 0x0),
    });

    p.values("event_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
