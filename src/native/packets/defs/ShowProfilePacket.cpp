#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ShowProfilePacket") {
    p.shape("1.16.201", {
        string("player_xuid", 0x0),
    });

    p.values("player_xuid", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
