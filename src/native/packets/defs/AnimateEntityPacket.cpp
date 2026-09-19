#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AnimateEntityPacket") {
    p.shape("1.16.201", {
        string("manimation", 0x18),
        string("mnextstate", 0x38),
        string("mstopexpression", 0x58),
        string("controller", 0x78),
        f32("blend_out_time", 0x98),
    });
    p.shape("1.17.30", {
        string("manimation", 0x18),
        string("mnextstate", 0x38),
        string("mstopexpression", 0x58),
        i32("stop_expression_molang_version", 0x78),
        string("mcontroller", 0x80),
        f32("mblendouttime", 0xA0),
    });
    p.shape("1.21.20", {
        string("manimation", 0x18),
        string("mnextstate", 0x38),
        string("mstopexpression", 0x58),
        i16("mstopexpressionversion", 0x78),
        string("mcontroller", 0x80),
        f32("mblendouttime", 0xA0),
    });
    p.shape("1.21.111", {
        string("manimation", 0x18),
        string("mnextstate", 0x38),
        string("mstopexpression", 0x58),
        i16("mstopexpressionversion", 0x78),
        string("mcontroller", 0x80),
    });
    p.shape("1.21.120", {
        string("manimation", 0x18),
        string("mnextstate", 0x38),
        string("mstopexpression", 0x58),
        i16("mstopexpressionversion", 0x78),
        string("mcontroller", 0x80),
        f32("mblendouttime", 0xA0),
    });

    p.values("manimation", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mnextstate", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mstopexpression", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("controller", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("blend_out_time", {0.0, 1.5, -1.0, 1e6});
    p.values("stop_expression_molang_version", {0, 1, -1, 1000, INT_MAX, INT_MIN});
    p.values("mcontroller", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mblendouttime", {0.0, 1.5, -1.0, 1e6});
    p.values("mstopexpressionversion", {0, 1, -1, 32767, -32768});
}
