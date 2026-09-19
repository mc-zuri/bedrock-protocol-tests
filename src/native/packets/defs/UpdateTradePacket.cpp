#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("UpdateTradePacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {u8("mContainerId", 0x0), i8("mType", 0x1), string("mDisplayName", 0x8), i32("mSize", 0x28), i32("mTraderTier", 0x2C),
                         i64("mEntityUniqueID", 0x30), i64("mLastTradingPlayer", 0x38), boolean("mUseNewTradeScreen", 0x58),
                         boolean("mUsingEconomyTrade", 0x59)});

    e::containerIdValues(p, "mContainerId");
    e::containerTypeValues(p, "mType");
    p.values("mDisplayName", {"", "entity.villager.armor", "\xc2\xa7" "aTrader \xe2\x9c\x93"});
    p.values("mSize", {0, 1, 10});
    p.values("mTraderTier", {0, 1, 2, 3, 4}); // Novice, Apprentice, Journeyman, Expert, Master
    p.values("mEntityUniqueID", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mLastTradingPlayer", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mUseNewTradeScreen", {false, true});
    p.values("mUsingEconomyTrade", {false, true});
}
