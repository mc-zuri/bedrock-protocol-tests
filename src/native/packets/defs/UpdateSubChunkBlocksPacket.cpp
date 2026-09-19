#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field blocks(char const* name, std::size_t at) {
    return vector(name, at, 0x28,
                  structure("value", 0, {blockPos("mPos", 0x0), u32("mRuntimeId", 0xC), u8("mUpdateFlags", 0x10), i64("mEntityUniqueID", 0x18),
                                         u32("mMessage", 0x20)}));
}
Object block(int x, int y, int z, std::int64_t runtimeId, int flags, std::int64_t entity, int message) {
    return Object{{"mPos", Object{{"x", x}, {"y", y}, {"z", z}}}, {"mRuntimeId", runtimeId}, {"mUpdateFlags", flags},
                  {"mEntityUniqueID", entity}, {"mMessage", message}};
}
}

BPX_PACKET("UpdateSubChunkBlocksPacket") {
    p.since("1.17.30");
    p.shape("1.17.30", {blocks("mStandards", 0x0), blocks("mExtras", 0x18), blockPos("mSubChunkBlockPosition", 0x30)});

    bpx::packets::enums::networkBlockPosValues(p, "mSubChunkBlockPosition");
    // BlockUpdateFlag bits (Neighbors 1, Network 2, NoGraphic 4, Priority 8); ActorBlockSyncMessage::MessageId: None,
    // Create, Destroy
    List const some{block(0, 64, 0, 1, 3, -1, 0), block(-15, 0, 15, 12345, 11, 42, 1), block(30000, 319, -30000, 0, 15, 7, 2)};
    p.values("mStandards", {List{}, some});
    p.values("mExtras", {List{}, List{block(1, 70, 1, 99, 2, -1, 0)}});
}
