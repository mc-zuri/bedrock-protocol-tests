#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("MovementEffectPacket") {
    p.since("1.21.50");
    p.shape("1.21.50", {u64("mRuntimeId", 0x0), i32("mEffectType", 0x8), i32("mEffectDuration", 0xC), u64("mTick", 0x10)});

    p.values("mRuntimeId", {1, 123456});
    p.values("mEffectType", {0});                     // GlideBoost
    p.values("1.21.60", "mEffectType", {0, 1});       // + DolphinBoost
    p.values("1.26.30", "mEffectType", {0, 1, 2});    // + GeyserBoost
    p.values("mEffectDuration", {0, 20, 600});
    p.values("mTick", {0, 1, 1234});
}
