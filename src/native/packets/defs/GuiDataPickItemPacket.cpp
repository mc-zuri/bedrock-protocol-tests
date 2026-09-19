#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("GuiDataPickItemPacket") {
    p.shape("1.16.201", {
        string("item_name", 0x0),
        string("item_effect_name", 0x20),
        i32("slot", 0x40),
    });

    p.values("item_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("item_effect_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("slot", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
