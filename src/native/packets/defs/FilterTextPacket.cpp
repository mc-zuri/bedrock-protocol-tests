#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("FilterTextPacket") {
    p.until("1.20.80");
    p.shape("1.16.201", {
        string("text", 0x0),
        boolean("from_server", 0x20),
    });

    p.values("text", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("from_server", {false, true});
}
