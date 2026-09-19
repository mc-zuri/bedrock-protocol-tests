#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("LevelEventPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {
        i32("mEventId", 0x0),
        vec3("mPos", 0x4),
        i32("mData", 0x10),
    });

    e::levelEventValues(p, "mEventId");
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mData", {0, 1, -1, 12345});
}
