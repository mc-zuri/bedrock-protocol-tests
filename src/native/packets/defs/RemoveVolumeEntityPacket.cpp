#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RemoveVolumeEntityPacket") {
    p.since("1.17.0");
    p.shape("1.17.0", {
        i32("dimension_type", 0x0),
    });
    p.shape("1.20.61", {
        i32("entity_network_id", 0x4),
    });
    p.shape("1.26.20", {
        i32("dimension_type", 0x0),
        i32("entity_network_id", 0x4),
    });

    p.values("dimension_type", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("entity_network_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
