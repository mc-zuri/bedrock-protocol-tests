#include "packets/Packet.h"

BPX_PACKET("UpdateClientOptionsPacket") {
    p.since("1.21.111");
    p.shape("1.21.111", {optional("mGraphicsMode", 0x0, 1, u8("value", 0))});
    p.shape("1.26.20", {optional("mGraphicsMode", 0x0, 1, u8("value", 0)), optional("mFilterProfanity", 0x2, 1, boolean("value", 0))});

    p.values("mGraphicsMode", {Value(), 0, 1, 2, 3}); // Simple, Fancy, Advanced, RayTraced
    p.values("mFilterProfanity", {Value(), false, true});
}
