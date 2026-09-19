#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CameraAimAssistPresetsPacket") {
    p.since("1.21.50");
    p.shape("1.21.50", {});
    p.shape("1.21.60", {
        u8("operation", 0x30),
    });
    p.shape("1.26.10", {
        u8("operation", 0x30),
    });

    p.values("operation", {0, 1, 2, 255});

    p.values("operation", {0, 1});
}
