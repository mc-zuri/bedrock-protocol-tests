#include "packets/Packet.h"

BPX_PACKET("UpdateClientInputLocksPacket") {
    p.since("1.19.50");
    p.shape("1.19.50", {u32("mInputLockComponentData", 0x0), vec3("mServerPosition", 0x4)});
    p.shape("1.26.10", {u32("mInputLockComponentData", 0x0)});

    List locks{0};
    // ClientInputLock: Camera, Movement, LateralMovement, Sneak, Jump, Mount, Dismount, MoveForward, MoveBackward,
    // MoveLeft, MoveRight
    for (int bit : {2, 4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000}) locks.push_back(bit);
    locks.push_back(0x1FF6); // all of them
    p.values("mInputLockComponentData", locks);
    p.values("mServerPosition", {Object{{"x", 0.5}, {"y", 65.62}, {"z", 0.5}}, Object{{"x", -12345.25}, {"y", -59.5}, {"z", 30000000.0}}});
}
