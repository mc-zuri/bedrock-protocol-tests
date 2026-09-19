#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ServerboundDiagnosticsPacket") {
    p.since("1.21.20");
    p.shape("1.21.20", {
        f32("avgfps", 0x0),
        f32("avgserversimticktimems", 0x4),
        f32("avgclientsimticktimems", 0x8),
        f32("avgbeginframetimems", 0xC),
        f32("avginputtimems", 0x10),
        f32("avgrendertimems", 0x14),
        f32("avgendframetimems", 0x18),
        f32("avgremaindertimepercent", 0x1C),
        f32("avgunaccountedtimepercent", 0x20),
    });
    p.shape("1.21.50", {
        f32("avgfps", 0x0),
        f32("avgserversimticktimems", 0x4),
        f32("avgclientsimticktimems", 0x8),
        f32("avgbeginframetimems", 0xC),
        f32("avginputtimems", 0x10),
        f32("avgrendertimems", 0x14),
        f32("avgendframetimems", 0x18),
        f32("avgremaindertimepercent", 0x24),
        f32("avgunaccountedtimepercent", 0x28),
    });
    p.shape("1.26.40", {
        f32("avgfps", 0x0),
        f32("avgserversimticktimems", 0x4),
        f32("avgclientsimticktimems", 0x8),
        f32("avgbeginframetimems", 0xC),
        f32("avginputtimems", 0x10),
        f32("avgrendertimems", 0x14),
        f32("avgendframetimems", 0x18),
        f32("avgremaindertimepercent", 0x24),
        f32("avgunaccountedtimepercent", 0x28),
        u8("has_system_categories", 0x90),
    });

    p.values("avgfps", {0.0, 1.5, -1.0, 1e6});
    p.values("avgserversimticktimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avgclientsimticktimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avgbeginframetimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avginputtimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avgrendertimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avgendframetimems", {0.0, 1.5, -1.0, 1e6});
    p.values("avgremaindertimepercent", {0.0, 1.5, -1.0, 1e6});
    p.values("avgunaccountedtimepercent", {0.0, 1.5, -1.0, 1e6});
    p.values("has_system_categories", {0, 1, 2, 255});

    // what the game reads back unchanged
    p.values("has_system_categories", {0, 1});
}
