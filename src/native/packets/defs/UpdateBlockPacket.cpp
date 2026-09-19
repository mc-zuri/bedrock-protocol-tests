#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("UpdateBlockPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {
        blockPos("mPos", 0x0),
        u32("mLayer", 0xC),
        u8("mUpdateFlags", 0x10),
        u32("mRuntimeId", 0x14),
    });

    e::networkBlockPosValues(p, "mPos");
    p.values("mLayer", {0, 1}); // 0 blocks, 1 liquids
    // BlockUpdateFlag bits: Neighbors 1, Network 2, NoGraphic 4, Priority 8
    p.values("mUpdateFlags", {0, 1, 2, 3, 4, 8, 11, 15});
    p.values("mRuntimeId", {0, 1, 12345, std::int64_t{2147483647}});
}
