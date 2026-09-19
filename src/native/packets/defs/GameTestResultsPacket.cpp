#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("GameTestResultsPacket") {
    p.since("1.19.30");
    p.shape("1.19.30", {
        string("testname", 0x0),
        string("error", 0x20),
        boolean("succeeded", 0x40),
    });
    p.shape("1.21.120", {});
    p.shape("1.21.130", {
        string("testname", 0x0),
        string("error", 0x20),
        boolean("succeeded", 0x40),
    });

    p.values("testname", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("error", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("succeeded", {false, true});
}
