#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("UnlockedRecipesPacket") {
    p.since("1.19.70");
    p.shape("1.19.70", {
        boolean("new_unlocks", 0x0),
    });
    p.shape("1.20.0", {
        i32("packet_type", 0x0),
    });

    p.values("new_unlocks", {false, true});
    p.values("packet_type", {0, 1, 2, 3, 4}); // Empty .. RemoveAllUnlockedRecipes
}
