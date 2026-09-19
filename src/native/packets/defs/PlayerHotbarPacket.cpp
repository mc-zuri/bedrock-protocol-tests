#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("PlayerHotbarPacket") {
    p.shape("1.16.201", {u32("mSelectedSlot", 0x0), boolean("mShouldSelectSlot", 0x4), u8("mContainerId", 0x5)});

    p.values("mSelectedSlot", {0, 1, 8});
    p.values("mShouldSelectSlot", {false, true});
    bpx::packets::enums::containerIdValues(p, "mContainerId");
}
