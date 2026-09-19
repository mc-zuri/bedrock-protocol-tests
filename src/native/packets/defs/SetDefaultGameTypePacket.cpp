#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetDefaultGameTypePacket") {
    p.shape("1.16.201", {
        i32("default_game_type", 0x0),
    });

    bpx::packets::enums::gameTypeValues(p, "default_game_type");

    p.baseline("default_game_type", 0);  // default -1 is not one the game takes
}
