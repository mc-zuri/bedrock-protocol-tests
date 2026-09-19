#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ModalFormRequestPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {
        i32("form_id", 0x0),
        string("form_ui_json", 0x8),
    });

    p.values("form_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("form_ui_json", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
