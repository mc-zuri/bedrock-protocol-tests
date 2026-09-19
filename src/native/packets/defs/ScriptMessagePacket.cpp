#include "packets/Packet.h"

BPX_PACKET("ScriptMessagePacket") {
    p.since("1.18.11");
    p.shape("1.18.11", {string("mMessageId", 0x0), string("mMessageValue", 0x20)});

    p.values("mMessageId", {"", "custom:event", "namespace:ping"});
    p.values("mMessageValue", {"", "{\"x\":1}", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
