#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AutomationClientConnectPacket") {
    p.since("1.21.60");
    p.shape("1.21.60", {
        string("web_socket_data", 0x0),
    });

    p.values("web_socket_data", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
