#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CurrentStructureFeaturePacket") {
    p.since("1.21.20");
    p.shape("1.21.20", {
        string("current_structure_feature", 0x0),
    });

    p.values("current_structure_feature", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
