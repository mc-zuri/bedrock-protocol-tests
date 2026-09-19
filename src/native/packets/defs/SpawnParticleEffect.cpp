#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SpawnParticleEffect") {
    p.until("1.20.50");
    p.shape("1.16.201", {
        u8("dimension", 0x0),
        i64("entity_unique_id", 0x8),
        f32("position_x", 0x10),
        f32("position_y", 0x14),
        f32("position_z", 0x18),
        string("particle_name", 0x20),
    });

    p.values("dimension", {0, 1, 2, 255});
    p.values("entity_unique_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("position_x", {0.0, 1.5, -1.0, 1e6});
    p.values("position_y", {0.0, 1.5, -1.0, 1e6});
    p.values("position_z", {0.0, 1.5, -1.0, 1e6});
    p.values("particle_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
