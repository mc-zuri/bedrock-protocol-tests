#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ClientboundDataDrivenUICloseScreenPacket") {
    p.since("1.26.10");
    p.shape("1.26.10", {optional("mFormId", 0x0, 4, u32("value", 0))});

    p.values("mFormId", {Value(), 0, 1, 12345, std::int64_t{4294967295LL}}); // absent: close every screen
}
