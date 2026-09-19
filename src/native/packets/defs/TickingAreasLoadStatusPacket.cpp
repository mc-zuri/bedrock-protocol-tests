#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("TickingAreasLoadStatusPacket") {
    p.since("1.18.30");
    p.shape("1.18.30", {
        boolean("waiting_for_preload", 0x0),
    });

    p.values("waiting_for_preload", {false, true});
}
