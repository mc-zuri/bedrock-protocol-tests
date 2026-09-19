#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ContainerClosePacket") {
    p.shape("1.16.201", {
        u8("container_id", 0x0),
        boolean("server_initiated_close", 0x1),
    });
    p.shape("1.21.0", {
        i32("container_id", 0x0),
    });
    p.shape("1.21.60", {
        u8("container_id", 0x0),
        u8("container_type", 0x1),
        boolean("server_initiated_close", 0x2),
    });

    bpx::packets::enums::containerIdValues(p, "container_id");
    p.values("server_initiated_close", {false, true});
    bpx::packets::enums::containerTypeValues(p, "container_type");
}
