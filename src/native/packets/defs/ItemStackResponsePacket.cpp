#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ItemStackResponsePacket") {
    p.since("1.21.60");
    p.shape("1.21.60", {});
    p.shape("1.26.40", {
    });
}
