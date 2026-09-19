#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetLastHurtByPacket") {
    p.shape("1.16.201", {
        i32("lasthurtby", 0x0),
    });

    p.values("lasthurtby", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
