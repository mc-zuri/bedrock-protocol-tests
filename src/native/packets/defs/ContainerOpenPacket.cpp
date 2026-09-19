#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ContainerOpenPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {u8("mContainerId", 0x0), i8("mType", 0x1), blockPos("mPos", 0x4), i64("mEntityUniqueID", 0x10)});

    e::containerIdValues(p, "mContainerId");
    e::containerTypeValues(p, "mType");
    e::networkBlockPosValues(p, "mPos");
    p.values("mEntityUniqueID", {-1, 1, std::int64_t{-4294967295LL}});
}
