#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("OnScreenTextureAnimationPacket") {
    p.shape("1.16.201", {
        i32("effect_id", 0x0),
    });

    p.values("effect_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
