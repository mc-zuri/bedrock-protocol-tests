#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AddBehaviorTreePacket") {
    p.shape("1.16.201", {
        string("behavior_tree_structure_json", 0x0),
    });

    p.values("behavior_tree_structure_json", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
