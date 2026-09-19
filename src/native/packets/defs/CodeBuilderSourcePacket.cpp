#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CodeBuilderSourcePacket") {
    p.since("1.18.11");
    p.shape("1.18.11", {
        u8("operation", 0x0),
        u8("category", 0x1),
        string("value", 0x8),
    });
    p.shape("1.21.0", {
        i32("operation", 0x0),
    });
    p.shape("1.21.60", {
        u8("operation", 0x0),
        u8("category", 0x1),
        u8("codestatus", 0x2),
    });

    p.values("operation", {0, 1, 2, 255});
    p.values("category", {0, 1, 2, 3, 4, 5, 6}); // Category: Menu .. Custom
    p.values("value", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("codestatus", {0, 1, 2, 3, 4, 5}); // CodeStatus: None .. Succeeded
}
