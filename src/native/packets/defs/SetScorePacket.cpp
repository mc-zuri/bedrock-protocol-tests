#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetScorePacket") {
    p.shape("1.16.201", {
        u8("score_packet_type", 0x0),
    });
    p.shape("1.26.40", {
    });

    p.values("score_packet_type", {0, 1, 2, 255});
}
