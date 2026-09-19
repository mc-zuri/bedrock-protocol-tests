#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ServerboundDataDrivenScreenClosedPacket") {
    p.since("1.26.10");
    p.shape("1.26.10", {
        u32("mFormId", 0x0),
        u8("mCloseReason", 0x4),
    });

    p.values("mFormId", {0, 1, 12345, std::int64_t{4294967295LL}});
    // ProgrammaticClose, ProgrammaticCloseAll, ClientCanceled, UserBusy, InvalidForm
    p.values("mCloseReason", {0, 1, 2, 3, 4});
}
