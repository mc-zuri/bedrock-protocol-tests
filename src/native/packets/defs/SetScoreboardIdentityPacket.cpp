#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetScoreboardIdentityPacket") {
    p.shape("1.16.201", {
        u8("scoreboard_identity_packet_type", 0x0),
    });

    p.values("scoreboard_identity_packet_type", {0, 1, 2, 255});

    // narrowed to what the game reads back unchanged
    p.values("scoreboard_identity_packet_type", {0, 1});
}
