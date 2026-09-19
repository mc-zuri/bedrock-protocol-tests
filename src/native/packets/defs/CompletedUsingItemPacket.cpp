#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CompletedUsingItemPacket") {
    p.shape("1.16.201", {
        i16("item_id", 0x0),
        i32("item_use_method", 0x4),
    });

    p.values("item_id", {0, 1, -1, 32767, -32768});
    p.values("item_use_method", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
