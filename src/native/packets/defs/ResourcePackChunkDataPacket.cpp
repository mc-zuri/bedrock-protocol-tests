#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ResourcePackChunkDataPacket") {
    p.shape("1.16.201", {
        string("resource_name", 0x0),
        i32("chunk_id", 0x20),
        i64("byte_offset", 0x28),
    });

    p.values("resource_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("chunk_id", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("byte_offset", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
