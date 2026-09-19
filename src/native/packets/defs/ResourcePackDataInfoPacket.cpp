#include "packets/Packet.h"

#include <cstdint>
#include <string>

BPX_PACKET("ResourcePackDataInfoPacket") {
    p.shape("1.16.201", {string("mResourceName", 0x0), u32("mChunkSize", 0x20), u32("mNumberOfChunks", 0x24),
                         u64("mFileSize", 0x28), string("mFileHash", 0x30), u8("mPackType", 0x50), boolean("mIsPremiumPack", 0x51)});

    p.values("mResourceName", {"", "0fba4063-dba1-4281-9b89-ff9390653530_1.0.0"});
    p.values("mChunkSize", {0, 1048576});
    p.values("mNumberOfChunks", {0, 1, 12});
    p.values("mFileSize", {0, 12345, std::int64_t{12582912}});
    p.values("mFileHash", {"", std::string(32, '\xab')});
    // Invalid, Addon, Cached, CopyProtected, Behavior, PersonaPiece, Resources, Skins, WorldTemplate
    p.values("mPackType", {0, 1, 2, 3, 4, 5, 6, 7, 8});
    p.values("mIsPremiumPack", {false, true});
}
