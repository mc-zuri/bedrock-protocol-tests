#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ClientCameraAimAssistPacket") {
    p.since("1.21.60");
    p.shape("1.21.60", {
        string("camera_preset_id", 0x0),
        u8("action", 0x20),
        boolean("allow_aim_assist", 0x21),
    });
    p.shape("1.26.10", {
        string("camera_preset_id", 0x0),
        u8("action", 0x20),
        boolean("allow_aim_assist", 0x21),
    });

    p.values("camera_preset_id", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("action", {0, 1, 2, 255});
    p.values("allow_aim_assist", {false, true});

    p.values("action", {0, 1});
}
