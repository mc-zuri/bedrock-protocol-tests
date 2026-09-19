#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("RespawnPacket") {
    p.shape("1.16.201", {
        vec3("mPos", 0x0),
        u8("mState", 0xC),
        u64("mRuntimeId", 0x10),
    });

    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mState", {0, 1, 2}); // SearchingForSpawn, ReadyToSpawn, ClientReadyToSpawn
    p.values("mRuntimeId", {0, 1, 123456, std::int64_t{1000000000000LL}});
}
