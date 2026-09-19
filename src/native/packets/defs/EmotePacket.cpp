#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("EmotePacket") {
    p.shape("1.16.201", {
        i64("actor_runtime_id", 0x0),
        string("emote_id", 0x8),
        u8("flags", 0x28),
    });
    p.shape("1.20.0", {
        i64("actor_runtime_id", 0x0),
        string("emote_id", 0x8),
        string("xuid", 0x28),
        string("platformid", 0x48),
        u8("flags", 0x68),
    });
    p.shape("1.21.30", {
        i64("actor_runtime_id", 0x0),
        string("emote_id", 0x8),
        i32("emote_length_ticks", 0x28),
        string("xuid", 0x30),
        string("platformid", 0x50),
        u8("flags", 0x70),
    });

    p.values("actor_runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("emote_id", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("flags", {0, 1, 2, 255});
    p.values("xuid", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("platformid", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("emote_length_ticks", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
