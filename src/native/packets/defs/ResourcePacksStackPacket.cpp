#include "packets/Packet.h"

BPX_PACKET("ResourcePacksStackPacket") {
    p.until("1.20.50");
    p.shape("1.16.201", {}); // this build's writer is only partly traced
    p.shape("1.16.210", {boolean("mTexturePackRequired", 0xA0)});
    p.shape("1.17.0", {boolean("mTexturePackRequired", 0xA8)});

    p.values("mTexturePackRequired", {false, true});
}
