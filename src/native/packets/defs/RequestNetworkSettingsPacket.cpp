#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RequestNetworkSettingsPacket") {
    p.since("1.19.30");
    p.shape("1.19.30", {
        i32("clientnetworkversion", 0x0),
    });

    p.values("clientnetworkversion", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
