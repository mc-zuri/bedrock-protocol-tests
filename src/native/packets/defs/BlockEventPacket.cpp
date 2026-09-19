#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("BlockEventPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {
        blockPos("mPos", 0x0),
        i32("mB0", 0xC),
        i32("mB1", 0x10),
    });

    e::networkBlockPosValues(p, "mPos");
    p.values("mB0", {0, 1}); // note block / bell / piston, chest lid (value: 1 open, 0 closed)
    p.values("mB1", {0, 1, 2, 24});
}
