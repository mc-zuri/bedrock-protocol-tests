#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ClientboundTextureShiftPacket") {
    p.since("1.26.0");
    p.shape("1.26.0", {
        u8("action_id", 0x0),
        string("collection_name", 0x8),
        string("from_step", 0x28),
        string("to_step", 0x48),
        i64("total_length_in_ticks", 0x80),
        i64("current_length_in_ticks", 0x88),
        boolean("enabled", 0x90),
    });

    p.values("action_id", {0, 1, 2, 3, 4}); // Action: Invalid, Initialize, Start, SetEnabled, Sync
    p.values("collection_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("from_step", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("to_step", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("total_length_in_ticks", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("current_length_in_ticks", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("enabled", {false, true});
}
