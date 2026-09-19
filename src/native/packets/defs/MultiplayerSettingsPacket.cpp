#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("MultiplayerSettingsPacket") {
    p.shape("1.16.201", {
        i32("packettype", 0x0),
    });

    p.values("packettype", {0, 1, 2}); // EnableMultiplayer, DisableMultiplayer, RefreshJoincode
}
