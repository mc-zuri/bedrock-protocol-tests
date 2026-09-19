#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetHudPacket") {
    p.since("1.20.61");
    p.shape("1.20.61", {
        u8("visibility", 0x18),
    });
    p.shape("1.21.70", {
        i32("hud_visible", 0x18),
    });

    p.values("visibility", {0, 1, 2, 255});
    p.values("hud_visible", {0, 1, -1, 1000, INT_MAX, INT_MIN});

    // narrowed to what the game reads back unchanged
    p.values("hud_visible", {0, 1});
}
