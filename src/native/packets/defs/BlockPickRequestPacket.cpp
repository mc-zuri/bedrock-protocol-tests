#include "packets/Packet.h"

BPX_PACKET("BlockPickRequestPacket") {
    p.shape("1.16.201", {
        blockPos("mPos", 0x0),
        boolean("mWithData", 0xC),
        u8("mMaxSlots", 0xD),
    });

    p.values("mPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                      Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mWithData", {false, true});
    p.values("mMaxSlots", {0, 1, 9}); // the hotbar size
}
