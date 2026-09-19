#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("LessonProgressPacket") {
    p.since("1.19.1");
    p.shape("1.19.1", {
        u8("lesson_action", 0x0),
        i32("score", 0x4),
        string("activity_id", 0x8),
    });

    p.values("lesson_action", {0, 1, 2}); // LessonAction: Start, Complete, Restart
    p.values("score", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("activity_id", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
