#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("GameRulesChangedPacket") {
    p.shape("1.16.201", {});
}
