#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SetCommandsEnabledPacket") {
    p.shape("1.16.201", {
        boolean("commands_enabled", 0x0),
    });

    p.values("commands_enabled", {false, true});
}
