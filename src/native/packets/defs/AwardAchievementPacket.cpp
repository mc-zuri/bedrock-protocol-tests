#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AwardAchievementPacket") {
    p.since("1.21.0");
    p.shape("1.21.0", {
        i32("achievementid", 0x0),
    });

    p.values("achievementid", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
