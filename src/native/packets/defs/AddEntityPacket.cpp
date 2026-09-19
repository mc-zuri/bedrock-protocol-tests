#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("AddEntityPacket") {
    p.until("1.20.61");
    p.shape("1.16.201", {u32("mEntityNetId", 0x0)});

    p.values("mEntityNetId", {0, 1, 12345, std::int64_t{4294967295LL}});
}
