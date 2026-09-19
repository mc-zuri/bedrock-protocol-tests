#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("ItemFrameDropItemPacket") {
    p.until("1.20.71");
    p.shape("1.16.201", {blockPos("mPos", 0x0)});

    bpx::packets::enums::networkBlockPosValues(p, "mPos");
}
