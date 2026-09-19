#include "packets/Packet.h"

BPX_PACKET("LecternUpdatePacket") {
    p.shape("1.16.201", {
        i32("mPage", 0x0),
        i32("mTotalPages", 0x4),
        boolean("mDropBook", 0x8),
        blockPos("mPos", 0xC),
    });
    p.shape("1.20.71", {
        i32("mPage", 0x0),
        i32("mTotalPages", 0x4),
        blockPos("mPos", 0x8),
    });

    p.values("mPage", {0, 1, 2, 49}); // a book holds up to 50 pages
    p.values("mTotalPages", {0, 1, 2, 50});
    p.values("mDropBook", {false, true});
    p.values("mPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                      Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
}
