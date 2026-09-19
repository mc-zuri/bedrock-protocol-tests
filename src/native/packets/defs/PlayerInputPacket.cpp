#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PlayerInputPacket") {
    p.until("1.21.80");
    p.shape("1.16.201", {
        f32("move_x", 0x0),
        f32("move_y", 0x4),
        boolean("jumping", 0x8),
        boolean("sneaking", 0x9),
    });

    p.values("move_x", {0.0, 1.5, -1.0, 1e6});
    p.values("move_y", {0.0, 1.5, -1.0, 1e6});
    p.values("jumping", {false, true});
    p.values("sneaking", {false, true});
}
