#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetDifficultyPacket") {
    p.shape("1.16.201", {
        i32("difficulty", 0x0),
    });

    p.values("difficulty", {0, 1, 2, 3}); // Peaceful, Easy, Normal, Hard
}
