#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("OpenSignPacket") {
    p.since("1.19.80");
    p.shape("1.19.80", {blockPos("mPos", 0x0), boolean("mIsFrontSide", 0xC)});

    bpx::packets::enums::networkBlockPosValues(p, "mPos");
    p.values("mIsFrontSide", {false, true});
}
