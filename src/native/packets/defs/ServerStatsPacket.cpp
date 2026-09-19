#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ServerStatsPacket") {
    p.since("1.19.30");
    p.shape("1.19.30", {
        f32("servertime", 0x0),
        f32("networktime", 0x4),
    });

    p.values("servertime", {0.0, 1.5, -1.0, 1e6});
    p.values("networktime", {0.0, 1.5, -1.0, 1e6});
}
