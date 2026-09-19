#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("PlayerActionPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {blockPos("mPos", 0x0), i32("mFace", 0xC), i32("mAction", 0x10), u64("mRuntimeId", 0x18)});
    p.shape("1.19.1", {blockPos("mPos", 0x0), blockPos("mResultPos", 0xC), i32("mFace", 0x18), i32("mAction", 0x1C), u64("mRuntimeId", 0x20)});

    e::networkBlockPosValues(p, "mPos");
    e::networkBlockPosValues(p, "mResultPos");
    p.values("mFace", {-1, 0, 1, 2, 3, 4, 5}); // none, Down, Up, North, South, West, East
    e::playerActionTypeValues(p, "mAction");
    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
}
