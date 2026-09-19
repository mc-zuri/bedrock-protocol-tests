#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("WSConnectPacket") {
    p.until("1.21.60");
    p.shape("1.16.201", {
        string("server_uri", 0x0),
    });

    p.values("server_uri", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
