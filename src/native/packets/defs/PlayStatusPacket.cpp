#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PlayStatusPacket") {
    p.shape("1.16.201", {
        i32("status", 0x0),
    });

    p.values("status", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
