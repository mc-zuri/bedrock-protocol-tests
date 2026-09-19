#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SimpleEventPacket") {
    p.shape("1.16.201", {
        i16("type", 0x0),
    });

    p.values("type", {0, 1, 2, 3}); // Uninitialized, EnableCommands, DisableCommands, UnlockWorldTemplateSettings
}
