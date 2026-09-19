#include "packets/Packet.h"

#include <climits>

BPX_PACKET("SetTimePacket") {
    p.shape("1.16.201", {i32("time", 0x0)});

    p.values("time", {0, 1, -1, 6000, 24000, INT_MAX, INT_MIN});
}
