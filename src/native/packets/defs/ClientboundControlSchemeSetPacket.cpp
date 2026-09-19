#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ClientboundControlSchemeSetPacket") {
    p.since("1.21.80");
    p.shape("1.21.80", {
        u8("control_scheme", 0x0),
    });

    p.values("control_scheme", {0, 1, 2, 255});

    p.values("control_scheme", {0, 1, 2, 3, 4});
}
