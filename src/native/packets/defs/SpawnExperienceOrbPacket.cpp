#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SpawnExperienceOrbPacket") {
    p.shape("1.16.201", {
        f32("position_x", 0x0),
        f32("position_y", 0x4),
        f32("position_z", 0x8),
        i32("xp_value", 0xC),
    });

    p.values("position_x", {0.0, 1.5, -1.0, 1e6});
    p.values("position_y", {0.0, 1.5, -1.0, 1e6});
    p.values("position_z", {0.0, 1.5, -1.0, 1e6});
    p.values("xp_value", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
