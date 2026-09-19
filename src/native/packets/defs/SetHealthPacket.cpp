#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetHealthPacket") {
    p.shape("1.16.201", {
        i32("health", 0x0),
    });

    p.values("health", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
