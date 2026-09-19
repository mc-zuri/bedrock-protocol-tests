#include "packets/Packet.h"

BPX_PACKET("PlayerListPacket") {
    p.shape("1.16.201", {u8("mAction", 0x18)});
    p.shape("1.26.40", {});

    p.values("mAction", {0, 1}); // Add, Remove
}
