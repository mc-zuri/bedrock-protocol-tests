#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("UpdatePlayerGameTypePacket") {
    p.shape("1.16.201", {
        i32("player_game_type", 0x0),
        i64("target_player", 0x8),
    });
    p.shape("1.20.80", {
        i32("player_game_type", 0x0),
        i64("target_player", 0x8),
        i32("player_replaystatecomponent_tick", 0x10),
    });
    p.shape("1.21.42", {
        i32("player_game_type", 0x0),
        i64("target_player", 0x8),
        i64("tick", 0x10),
    });

    bpx::packets::enums::gameTypeValues(p, "player_game_type");
    p.values("target_player", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("player_replaystatecomponent_tick", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("tick", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
