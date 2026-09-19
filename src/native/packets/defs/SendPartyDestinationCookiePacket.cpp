#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SendPartyDestinationCookiePacket") {
    p.since("1.26.30");
    p.shape("1.26.30", {
        string("cookie", 0x0),
        u8("intent", 0x20),
        string("destination_name", 0x28),
    });

    p.values("cookie", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("intent", {0, 1, 2, 255});
    p.values("destination_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});

    // what the game reads back unchanged
    p.values("intent", {0, 1, 2});
}
