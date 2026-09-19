#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetPlayerGameTypePacket") {
    p.shape("1.16.201", {
        i32("player_game_type", 0x0),
    });

    bpx::packets::enums::gameTypeValues(p, "player_game_type");
}
