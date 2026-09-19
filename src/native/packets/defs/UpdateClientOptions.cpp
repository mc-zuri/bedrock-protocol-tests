#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("UpdateClientOptions") {
    p.since("1.21.70");
    p.until("1.21.111");
    p.shape("1.21.70", {
        u8("new_graphics_mode_value", 0x0),
        boolean("graphics_mode_change", 0x1),
    });

    p.values("new_graphics_mode_value", {0, 1, 2, 255});
    p.values("graphics_mode_change", {false, true});

    p.scenario("new_graphics_mode_value when graphics_mode_change=1", {{"graphics_mode_change", 1}, {"new_graphics_mode_value", 1}});
}
