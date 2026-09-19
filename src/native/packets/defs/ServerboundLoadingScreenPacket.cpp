#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ServerboundLoadingScreenPacket") {
    p.since("1.21.20");
    p.shape("1.21.20", {
        i32("mServerboundLoadingScreenPacketType", 0x0),
        optional("mLoadingScreenId", 0x4, 4, u32("value", 0)),
    });

    p.baseline("mServerboundLoadingScreenPacketType", 1);
    p.values("mServerboundLoadingScreenPacketType", {1, 2}); // StartLoadingScreen, EndLoadingScreen (Unknown 0 is not read back)
    p.values("mLoadingScreenId", {Value(), 0, 1, std::int64_t{4294967295LL}});
}
