#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("UpdateEquipPacket") {
    p.unnamed(81);
    p.shape("1.16.201", {
        u8("container_id", 0x0),
        u8("type", 0x1),
        i32("size", 0x4),
        i64("entity_unique_id", 0x8),
    });

    bpx::packets::enums::containerIdValues(p, "container_id");
    bpx::packets::enums::containerTypeValues(p, "type");
    p.values("size", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("entity_unique_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
