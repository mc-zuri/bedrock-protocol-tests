#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ResourcePackChunkRequestPacket") {
    p.shape("1.16.201", {string("mResourceName", 0x0), u32("mChunk", 0x20)});

    p.values("mResourceName", {"", "0fba4063-dba1-4281-9b89-ff9390653530_1.0.0", "5ff43d1b-2d8a-4f7c-8f2b-3e2f0f8e1a11_2.3.4"});
    p.values("mChunk", {0, 1, 42, std::int64_t{4294967295LL}});
}
