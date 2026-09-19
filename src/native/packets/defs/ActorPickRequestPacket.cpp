#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ActorPickRequestPacket") {
    p.shape("1.16.201", {i64("mID", 0x0), u8("mMaxSlots", 0x8)});
    p.shape("1.17.30", {i64("mID", 0x0), boolean("mWithData", 0x8), u8("mMaxSlots", 0x9)});

    p.values("mID", {1, std::int64_t{-4294967295LL}});
    p.values("mWithData", {false, true});
    p.values("mMaxSlots", {0, 1, 9}); // the hotbar size
}
