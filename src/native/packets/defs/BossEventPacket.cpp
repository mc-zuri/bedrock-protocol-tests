#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("BossEventPacket") {
    namespace e = bpx::packets::enums;
    Layout l{i64("mBossID", 0x8), i64("mPlayerID", 0x10), i32("mEventType", 0x18)};
    p.shape("1.16.201", l);
    for (Field f : {string("mName", 0x20), optional("mFilteredName", 0x40, 0x20, string("value", 0)), f32("mHealthPercent", 0x68),
                    u8("mColor", 0x6C), u8("mOverlay", 0x6D)})
        l.push_back(std::move(f));
    p.shape("1.26.40", l);
    p.shape("1.26.50", {i64("mBossID", 0x0), u8("mEventType", 0x8), string("mName", 0x10),
                        optional("mFilteredName", 0x30, 0x20, string("value", 0)), f32("mHealthPercent", 0x58), u8("mColor", 0x5C),
                        u8("mOverlay", 0x5D)});

    p.values("mBossID", {1, std::int64_t{-4294967295LL}});
    p.values("mPlayerID", {-1, 1, std::int64_t{-8589934591LL}});
    // BossEventUpdateType: Add, PlayerAdded, Remove, PlayerRemoved, UpdatePercent, UpdateName, UpdateProperties,
    // UpdateStyle; Query (8) is read back from 1.18.11
    p.values("mEventType", e::range(0, 7));
    p.values("1.18.11", "mEventType", e::range(0, 8));
    p.values("mName", {"", "Ender Dragon", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mFilteredName", {Value(), "E**** Dragon"});
    p.values("mHealthPercent", {0.0, 0.5, 1.0});
    p.values("mColor", e::range(0, 7));   // BossBarColor: Pink .. White
    p.values("mOverlay", e::range(0, 4)); // BossBarOverlay: Progress, Notched 6 / 10 / 12 / 20
}
