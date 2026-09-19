#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("TelemetryEventPacket") {
    p.until("1.20.30");
    p.shape("1.16.201", {i64("mPlayerUniqueId", 0x0), i32("mType", 0x8), u8("mUsePlayerID", 0xC), i32("mAchievementId", 0x10)});

    p.values("mPlayerUniqueId", {1, std::int64_t{-4294967295LL}, std::int64_t{123456789012LL}});
    p.values("mUsePlayerID", {0, 1});
    List types;
    for (int t = 0; t <= 29; ++t) types.push_back(t); // AchievementAwarded .. SneakCloseToSculkSensor
    p.values("mType", types);
    // MinecraftEventing::AchievementIds as of 1.20.81
    List achievements;
    for (int a : {7,  10, 20, 21, 29, 30, 37, 38, 39, 40, 50, 52, 53, 54, 56, 58, 60, 61, 62, 63, 64, 65, 66,  67,  68,  69,
                  71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
                  97, 98, 99, 100, 101, 102, 103, 104, 105, 106})
        achievements.push_back(a);
    p.values("mAchievementId", achievements);
}
