#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ContainerSetDataPacket") {
    p.shape("1.16.201", {
        i64("id", 0x0),
        u8("container_id", 0x8),
    });
    p.shape("1.21.60", {
        i32("id", 0x0),
        i32("value", 0x4),
        u8("container_id", 0x8),
    });
    p.shape("1.21.100", {
        u8("container_id", 0x0),
        i32("id", 0x4),
        i32("value", 0x8),
    });

    p.values("id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    bpx::packets::enums::containerIdValues(p, "container_id");
    p.values("value", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
