#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("DebugInfoPacket") {
    p.shape("1.16.201", {
        string("data", 0x0),
        i64("actor_id", 0x20),
    });

    p.values("data", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("actor_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
