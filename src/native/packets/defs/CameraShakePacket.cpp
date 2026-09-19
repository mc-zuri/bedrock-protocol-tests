#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CameraShakePacket") {
    p.shape("1.16.201", {
        f32("intensity", 0x0),
        f32("seconds", 0x4),
        u8("shake_type", 0x8),
    });
    p.shape("1.16.210", {
        f32("intensity", 0x0),
        f32("seconds", 0x4),
        u8("shake_type", 0x8),
        u8("shake_action", 0x9),
    });
    p.shape("1.17.30", {});
    p.shape("1.17.40", {
        f32("intensity", 0x0),
        f32("seconds", 0x4),
        u8("shake_type", 0x8),
        u8("shake_action", 0x9),
    });

    p.values("intensity", {0.0, 1.5, -1.0, 1e6});
    p.values("seconds", {0.0, 1.5, -1.0, 1e6});
    p.values("shake_type", {0, 1, 2, 255});
    p.values("shake_action", {0, 1, 2, 255});

    p.values("shake_action", {0, 1});
    p.values("shake_type", {0, 1});
}
