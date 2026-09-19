#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("AnvilDamagePacket") {
    p.shape("1.16.201", {i32("mDamage", 0x0), blockPos("mPosition", 0x4)});
    p.shape("1.26.40", {blockPos("mPosition", 0x0)});

    p.values("mDamage", {0, 1, 2, 3}); // damage steps: the anvil breaks after the third
    bpx::packets::enums::networkBlockPosValues(p, "mPosition");
}
