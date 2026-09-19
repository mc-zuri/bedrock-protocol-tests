#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ClientCacheBlobStatusPacket") {
    p.shape("1.16.201", {
        vector("mMissingIds", 0x0, 8, u64("value", 0)),
        vector("mFoundIds", 0x18, 8, u64("value", 0)),
    });

    // blob ids are xxHash64 values of chunk blobs: any 64-bit number
    List const one{std::int64_t{0x1234567890ABCDEFLL}};
    List const some{std::int64_t{1}, std::int64_t{0x1234567890ABCDEFLL}, std::int64_t{-1}};
    p.values("mMissingIds", {List{}, one, some});
    p.values("mFoundIds", {List{}, one, some});
}
