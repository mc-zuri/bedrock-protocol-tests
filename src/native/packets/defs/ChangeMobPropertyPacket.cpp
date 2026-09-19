#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ChangeMobPropertyPacket") {
    p.since("1.18.30");
    p.shape("1.18.30", {
        string("mPropName", 0x0),
        boolean("mBoolComponentVal", 0x20),
        f32("mFloatComponentVal", 0x24),
        i32("mIntComponentVal", 0x28),
        string("mStringComponentVal", 0x30),
        i64("mActorId", 0x50),
    });

    p.values("mPropName", {"", "minecraft:has_nectar", "custom:state"});
    p.values("mBoolComponentVal", {false, true});
    p.values("mFloatComponentVal", {0.0, 0.5, -2.25, 100.0});
    p.values("mIntComponentVal", {0, 1, -5, 1000});
    p.values("mStringComponentVal", {"", "open", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mActorId", {-1, 1, std::int64_t{-4294967295LL}});
}
