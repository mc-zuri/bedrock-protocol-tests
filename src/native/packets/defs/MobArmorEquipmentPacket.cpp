#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("MobArmorEquipmentPacket") {
    p.shape("1.16.201", {
        i64("target_runtime_id", 0x0),
    });
    p.shape("1.16.220", {
        i64("entity_runtime_id", 0x200),
    });
    p.shape("1.18.30", {
        i64("entity_runtime_id", 0x260),
    });
    p.shape("1.19.20", {
        i64("entity_runtime_id", 0x160),
    });
    p.shape("1.19.30", {
        i64("entity_runtime_id", 0x180),
    });
    p.shape("1.20.40", {});
    p.shape("1.26.30", {
        i64("target_runtime_id", 0x0),
    });

    p.values("target_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("entity_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
