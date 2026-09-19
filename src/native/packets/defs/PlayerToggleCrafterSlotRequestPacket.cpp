#include "packets/Packet.h"

BPX_PACKET("PlayerToggleCrafterSlotRequestPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {
        blockPos("mPos", 0x0),
        i32("mSlotIndex", 0xC),
        boolean("mIsDisabled", 0x10),
    });

    p.values("mPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                      Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mSlotIndex", {0, 1, 4, 8}); // the crafter's 3x3 grid
    p.values("mIsDisabled", {false, true});
}
