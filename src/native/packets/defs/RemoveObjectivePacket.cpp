#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RemoveObjectivePacket") {
    p.shape("1.16.201", {
        string("objective_name", 0x0),
    });

    p.values("objective_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
