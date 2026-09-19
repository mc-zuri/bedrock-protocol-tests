#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PlayerStartItemCooldownPacket") {
    p.since("1.18.11");
    p.shape("1.18.11", {
        string("item_category", 0x0),
        i32("duration_ticks", 0x20),
    });

    p.values("item_category", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("duration_ticks", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
