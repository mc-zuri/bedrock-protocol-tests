#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("MobEffectPacket") {
    namespace e = bpx::packets::enums;
    Layout base{u64("mRuntimeId", 0x0), i32("mEffectDurationTicks", 0x8), u8("mEventId", 0xC), i32("mEffectId", 0x10),
                i32("mEffectAmplifier", 0x14), boolean("mShowParticles", 0x18)};
    p.shape("1.16.201", base);
    base.push_back(u64("mTick", 0x20));
    p.shape("1.20.71", base);
    base.push_back(boolean("mAmbient", 0x19));
    p.shape("1.21.130", base);

    p.baseline("mEventId", 1);
    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mEffectDurationTicks", {0, 20, 600, 2147483647}); // ticks; the largest is "infinite"
    p.values("mEventId", {1, 2, 3});                          // Event: Add, Update, Remove (Invalid 0 is not sent)
    p.values("mEffectId", e::range(1, 29));            // Speed .. HeroOfTheVillage
    p.values("1.19.1", "mEffectId", e::range(1, 30));  // + Darkness
    p.values("1.21.0", "mEffectId", e::range(1, 36));  // + TrialOmen .. RaidOmen
    p.values("mEffectAmplifier", {0, 1, 4, 255});
    p.values("mShowParticles", {false, true});
    p.values("mTick", {0, 1, 1234});
    p.values("mAmbient", {false, true});
}
