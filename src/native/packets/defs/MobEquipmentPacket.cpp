#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("MobEquipmentPacket") {
    p.shape("1.16.201", {
        i64("target_runtime_id", 0x0),
    });
    p.shape("1.20.40", {});
    p.shape("1.26.20", {
        i64("target_runtime_id", 0x0),
        i32("slot", 0x68),
        i32("selected_slot", 0x6C),
        u8("container_id", 0x70),
    });

    p.values("target_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("slot", {0, 1, 8, 9, 35}); // an inventory slot (hotbar 0 .. 8, inventory 9 .. 35)
    p.values("selected_slot", {0, 1, 8}); // a hotbar slot
    bpx::packets::enums::containerIdValues(p, "container_id");
}
