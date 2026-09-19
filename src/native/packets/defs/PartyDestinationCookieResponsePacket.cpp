#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("PartyDestinationCookieResponsePacket") {
    p.since("1.26.30");
    p.shape("1.26.30", {
        string("cookie", 0x0),
        boolean("accepted", 0x20),
    });

    p.values("cookie", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("accepted", {false, true});
}
