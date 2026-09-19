#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("MoveActorDeltaPacket") {
    p.shape("1.16.201", {u64("mRuntimeId", 0x0), u16("mHeader", 0x8), f32("mNewPositionX", 0xC), f32("mNewPositionY", 0x10),
                         f32("mNewPositionZ", 0x14), i8("mRotX", 0x18), i8("mRotY", 0x19), i8("mRotYHead", 0x1A)});

    p.baseline("mHeader", 0x3F); // position and rotations sent: every value below goes on the wire
    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    // HasX 1, HasY 2, HasZ 4, HasRotX 8, HasRotY 0x10, HasRotYHead 0x20, OnGround 0x40, Teleport 0x80, ForceMove 0x100
    p.values("mHeader", {0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x3F, 0x1FF});
    p.values("mNewPositionX", {0.0, -123.5, 30000.25});
    p.values("mNewPositionY", {64.0, -60.0, 319.0});
    p.values("mNewPositionZ", {0.0, 456.75, -30000.25});
    p.values("mRotX", {0, 32, -64, 127});
    p.values("mRotY", {0, 64, -128});
    p.values("mRotYHead", {0, 64, -128});
}
