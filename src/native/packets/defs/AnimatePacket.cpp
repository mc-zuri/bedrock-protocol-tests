#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("AnimatePacket") {
    p.shape("1.16.201", {u64("mRuntimeId", 0x0), i32("mAction", 0x8), f32("mData", 0xC)});
    p.shape("1.21.130", {u64("mRuntimeId", 0x0), u8("mAction", 0x8), f32("mData", 0xC), optional("mSwingSource", 0x10, 1, u8("value", 0))});

    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mAction", {0, 1, 3, 4, 5}); // NoAction, Swing, WakeUp, CriticalHit, MagicCriticalHit
    for (int a : {128, 129}) p.scenario("1.16.201", "1.21.130", "rowing", {{"mAction", a}, {"mData", 1.25}}); // RowRight, RowLeft
    p.values("1.21.120", "mData", {0.0, 1.25}); // before 1.21.120 mData is sent only with the rowing actions
    p.values("mSwingSource", {Value(), 0, 1, 2, 3, 4, 5, 6, 7, 8}); // ActorSwingSource: None .. Event
}
