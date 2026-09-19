#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ServerPlayerPostMovePositionPacket") {
    p.since("1.21.20");
    p.shape("1.21.20", {
        f32("pos_x", 0x0),
        f32("pos_y", 0x4),
        f32("pos_z", 0x8),
    });

    p.values("pos_x", {0.0, 1.5, -1.0, 1e6});
    p.values("pos_y", {0.0, 1.5, -1.0, 1e6});
    p.values("pos_z", {0.0, 1.5, -1.0, 1e6});
}
