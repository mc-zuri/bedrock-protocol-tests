#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AvailableCommandsPacket") {
    p.shape("1.16.201", {});
}
