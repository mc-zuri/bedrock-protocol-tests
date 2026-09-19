#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("SetSpawnPositionPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {
        blockPos("mPos", 0x0),
        i32("mSpawnPosType", 0xC),
        i32("mDimensionType", 0x10),
        blockPos("mSpawnBlockPos", 0x14),
    });

    e::networkBlockPosValues(p, "mPos");
    e::networkBlockPosValues(p, "mSpawnBlockPos");
    p.values("mSpawnPosType", {0, 1});     // PlayerRespawn, WorldSpawn
    p.values("mDimensionType", {0, 1, 2}); // Overworld, Nether, TheEnd
}
