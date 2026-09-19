#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CraftingEventPacket") {
    p.until("1.20.50");
    p.shape("1.16.201", {
        u8("window_id", 0x0),
        i32("crafting_type", 0x4),
        i64("m_8", 0x8),
        i64("m_10", 0x10),
    });

    p.values("window_id", {0, 1, 2, 255});
    p.values("crafting_type", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("m_8", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("m_10", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
