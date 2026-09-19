#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PassengerJumpPacket") {
    p.since("1.17.40");
    p.until("1.21.80");
    p.shape("1.17.40", {
        i32("jump_scale", 0x0),
    });

    p.values("jump_scale", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
