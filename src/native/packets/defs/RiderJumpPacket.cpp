#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RiderJumpPacket") {
    p.until("1.17.40");
    p.shape("1.16.201", {
        i32("jump_strength", 0x0),
    });

    p.values("jump_strength", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
