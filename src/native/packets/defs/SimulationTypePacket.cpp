#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("SimulationTypePacket") {
    p.since("1.17.10");
    p.shape("1.17.10", {
        u8("sim_type", 0x0),
    });

    p.values("sim_type", {0, 1, 2}); // Game, Editor, Test
}
