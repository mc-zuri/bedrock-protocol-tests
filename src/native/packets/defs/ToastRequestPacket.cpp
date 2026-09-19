#include "packets/Packet.h"

BPX_PACKET("ToastRequestPacket") {
    p.since("1.19.1");
    p.shape("1.19.1", {string("mTitle", 0x0), string("mContent", 0x20)});

    p.values("mTitle", {"", "Achievement get!", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mContent", {"", "You did it.", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
