#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("CreatePhotoPacket") {
    p.since("1.17.30");
    p.shape("1.17.30", {
        string("photo_item_name", 0x0),
        i64("raw_id", 0x20),
        string("photo_name", 0x28),
    });
    p.shape("1.21.111", {
        i64("raw_id", 0x0),
        string("photo_name", 0x8),
        string("photo_item_name", 0x28),
    });

    p.values("photo_item_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("raw_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("photo_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
