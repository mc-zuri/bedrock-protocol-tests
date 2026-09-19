#include "packets/Packet.h"

#include <cstdint>
#include <string>
#include <utility>

namespace {
using namespace bpx::schema;

// MinecraftEventing::AchievementIds as of 1.26.40
constexpr int kAchievements[] = {7,   10,  20,  21,  29,  30,  37,  38,  39,  40,  50,  52,  53,  54,  56,  58,
                                 60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  71,  72,  73,  74,  75,  76,
                                 77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,
                                 93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108,
                                 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124,
                                 125, 126};

char const* achievementSince(int a) {
    return a <= 106 ? "0.0.0" : a <= 113 ? "1.21.50" : a == 114 ? "1.21.90" : a == 115 ? "1.21.130" : a == 116 ? "1.26.10"
         : a == 117 ? "1.26.30" : "1.26.40";
}

// in the variant's declaration order: the value is the variant index
enum Alt : int {
    Achievement, Interaction, PortalCreated, PortalUsed, MobKilled, CauldronUsed, PlayerDied, BossKilled,
    SlashCommand, MobBorn, POICauldronUsed, ComposterUsed, BellUsed, ActorDefinition, RaidUpdate, TargetBlockHit,
    PiglinBarter, PlayerWaxedOrUnwaxedCopper, CodeBuilderRuntimeAction, CodeBuilderScoreboard, ItemUsed, Empty,
};

Field eventData() {
    return variant("mEventData", 0x10, 0x48,
        {
            structure("Achievement", 0, {u8("mAchievementId", 0x0)}),
            structure("Interaction", 0, {i64("mInteractedEntityId", 0x0), i32("mInteractedEntityType", 0x8),
                                         u8("mInteractionType", 0xC), i32("mInteractedEntityVariant", 0x10),
                                         u8("mInteractedEntityColor", 0x14)}),
            structure("PortalCreated", 0, {i32("mBuiltInDimension", 0x0)}),
            structure("PortalUsed", 0, {i32("mFromDimension", 0x0), i32("mToDimension", 0x4)}),
            structure("MobKilled", 0, {i64("mKillerEntityId", 0x0), i64("mKilledMobId", 0x8), i32("mDamageChildType", 0x10),
                                       i32("mDamageSource", 0x14), i32("mTraderTier", 0x18), string("mTraderName", 0x20)}),
            structure("CauldronUsed", 0, {i16("mContentsType", 0x0), u32("mContentsColor", 0x4), i16("mFillLevel", 0x8)}),
            structure("PlayerDied", 0, {i32("mKillerId", 0x0), i32("mKillerVariant", 0x4), i32("mDamageSource", 0x8),
                                        boolean("mInRaid", 0xC)}),
            structure("BossKilled", 0, {i32("mPartySize", 0x0), i64("mBossUniqueId", 0x8), i32("mBossType", 0x10)}),
            structure("SlashCommand", 0, {string("mCommandName", 0x0), i32("mSuccessCount", 0x20), i32("mErrorCount", 0x24),
                                          string("mErrorList", 0x28)}),
            structure("MobBorn", 0, {i32("mBabyType", 0x0), i32("mBabyVariant", 0x4), u8("mBabyColor", 0x8)}),
            structure("POICauldronUsed", 0, {i16("mItemId", 0x0), u8("mInteractionType", 0x2)}),
            structure("ComposterUsed", 0, {i16("mItemId", 0x0), u8("mInteractionType", 0x2)}),
            structure("BellUsed", 0, {i16("mItemId", 0x0)}),
            structure("ActorDefinition", 0, {string("mEventName", 0x0)}),
            structure("RaidUpdate", 0, {i32("mCurrentWave", 0x0), i32("mTotalWaves", 0x4), boolean("mSuccess", 0x8)}),
            structure("TargetBlockHit", 0, {i32("mRedstoneLevel", 0x0)}),
            structure("PiglinBarter", 0, {i32("mItemId", 0x0), boolean("mWasTargetingBarteringPlayer", 0x4)}),
            structure("PlayerWaxedOrUnwaxedCopper", 0, {i32("mBlockID", 0x0)}),
            structure("CodeBuilderRuntimeAction", 0, {string("mRuntimeAction", 0x0)}),
            structure("CodeBuilderScoreboard", 0, {string("mObjectiveName", 0x0), i32("mScore", 0x20)}),
            structure("ItemUsed", 0, {i16("mItemId", 0x0), i32("mItemAux", 0x4), i32("mUseMethod", 0x8), i32("mCount", 0xC)}),
            structure("Empty", 0, {}),
        });
}

Object event(int type, Alt alt, Object data) {
    return Object{{"mType", type}, {"mEventData", Object{{"index", static_cast<int>(alt)}, {"value", std::move(data)}}}};
}
}

BPX_PACKET("LegacyTelemetryEventPacket") {
    p.since("1.20.30");
    p.shape("1.20.30", {
        i64("mPlayerUniqueId", 0x0),
        i32("mType", 0x8),
        u8("mUsePlayerID", 0xC),
        i32("mAchievementId", 0x10),
    });
    p.shape("1.21.130", {
        i64("mPlayerUniqueId", 0x0),
        i32("mType", 0x8),
        boolean("mUsePlayerID", 0xC),
        eventData(),
    });

    p.values("mPlayerUniqueId", {1, -4294967295LL, 123456789012LL});
    p.values("mUsePlayerID", {0, 1});

    {
        List types;
        for (int t = 0; t <= 31; ++t) types.push_back(t);
        p.values("1.20.30", "mType", types);
        for (char const* since : {"0.0.0", "1.21.50", "1.21.90"}) {
            List achievements;
            for (int a : kAchievements)
                if (bpx::Version::parse(achievementSince(a)) <= bpx::Version::parse(since)) achievements.push_back(a);
            p.values(since, "mAchievementId", achievements);
        }
    }
    // 1.21.130+: the writer takes the data from the alternative mType names, so mType only goes with its event data
    p.values("1.21.130", "mType", {});

    // Type: Achievement 0 .. ItemUsed 31
    for (int a : kAchievements)
        p.scenario(achievementSince(a), "Achievement " + std::to_string(a), event(0, Achievement, {{"mAchievementId", a}}));
    for (int i = 1; i <= 17; ++i) // MinecraftEventing::InteractionType: Breeding 1 .. Equipping 17 (1.26.30+)
        p.scenario(i == 17 ? "1.26.30" : "0.0.0", "Interaction " + std::to_string(i),
            event(1, Interaction, {{"mInteractedEntityId", -4294967295LL}, {"mInteractedEntityType", 21262}, {"mInteractionType", i},
                                   {"mInteractedEntityVariant", i % 3}, {"mInteractedEntityColor", i % 16}}));
    for (int d = 0; d <= 2; ++d) // dimensions: Overworld 0, Nether 1, TheEnd 2
        p.scenario("PortalCreated " + std::to_string(d), event(2, PortalCreated, {{"mBuiltInDimension", d}}));
    p.scenario("PortalUsed 0>1", event(3, PortalUsed, {{"mFromDimension", 0}, {"mToDimension", 1}}));
    p.scenario("PortalUsed 1>2", event(3, PortalUsed, {{"mFromDimension", 1}, {"mToDimension", 2}}));
    p.scenario("MobKilled zombie", event(4, MobKilled, {{"mKillerEntityId", -4294967295LL}, {"mKilledMobId", 123456},
                                                        {"mDamageChildType", 12582992}, {"mDamageSource", 2}, {"mTraderTier", 0},
                                                        {"mTraderName", ""}}));
    p.scenario("MobKilled trader", event(4, MobKilled, {{"mKillerEntityId", 1}, {"mKilledMobId", 2}, {"mDamageChildType", 319},
                                                        {"mDamageSource", 1}, {"mTraderTier", 4}, {"mTraderName", "Librarian"}}));
    p.scenario("CauldronUsed", event(5, CauldronUsed, {{"mContentsType", 1}, {"mContentsColor", 0xFF3F76E4LL}, {"mFillLevel", 6}}));
    p.scenario("PlayerDied", event(6, PlayerDied, {{"mKillerId", 199456}, {"mKillerVariant", 0}, {"mDamageSource", 2}, {"mInRaid", false}}));
    p.scenario("PlayerDied raid", event(6, PlayerDied, {{"mKillerId", 2849}, {"mKillerVariant", 1}, {"mDamageSource", 10}, {"mInRaid", true}}));
    p.scenario("BossKilled", event(7, BossKilled, {{"mPartySize", 4}, {"mBossUniqueId", -4294967295LL}, {"mBossType", 53}}));
    for (int t : {8, 9, 10, 12, 14, 20, 21, 22, 28, 29, 30}) // no data (obsolete / name-only events)
        p.scenario("Empty " + std::to_string(t), event(t, Empty, {}));
    p.scenario("SlashCommand", event(11, SlashCommand, {{"mCommandName", "give"}, {"mSuccessCount", 1}, {"mErrorCount", 0}, {"mErrorList", ""}}));
    p.scenario("SlashCommand errors", event(11, SlashCommand, {{"mCommandName", "tp"}, {"mSuccessCount", 0}, {"mErrorCount", 2},
                                                              {"mErrorList", "commands.generic.noTargetMatch"}}));
    p.scenario("MobBorn", event(13, MobBorn, {{"mBabyType", 4876}, {"mBabyVariant", 0}, {"mBabyColor", 0}}));
    p.scenario("MobBorn colored", event(13, MobBorn, {{"mBabyType", 13}, {"mBabyVariant", 2}, {"mBabyColor", 14}}));
    for (int i = 0; i <= 25; ++i) { // MinecraftEventing::POIBlockInteractionType: None 0 .. DisenchantAndRepair 25
        p.scenario("POICauldronUsed " + std::to_string(i), event(15, POICauldronUsed, {{"mItemId", 373}, {"mInteractionType", i}}));
        p.scenario("ComposterUsed " + std::to_string(i), event(16, ComposterUsed, {{"mItemId", 296}, {"mInteractionType", i}}));
    }
    p.scenario("BellUsed", event(17, BellUsed, {{"mItemId", 0}}));
    p.scenario("BellUsed item", event(17, BellUsed, {{"mItemId", 280}}));
    p.scenario("ActorDefinition", event(18, ActorDefinition, {{"mEventName", "minecraft:entity_born"}}));
    p.scenario("RaidUpdate", event(19, RaidUpdate, {{"mCurrentWave", 1}, {"mTotalWaves", 5}, {"mSuccess", false}}));
    p.scenario("RaidUpdate won", event(19, RaidUpdate, {{"mCurrentWave", 5}, {"mTotalWaves", 5}, {"mSuccess", true}}));
    for (int r : {0, 1, 15}) p.scenario("TargetBlockHit " + std::to_string(r), event(23, TargetBlockHit, {{"mRedstoneLevel", r}}));
    p.scenario("PiglinBarter", event(24, PiglinBarter, {{"mItemId", 266}, {"mWasTargetingBarteringPlayer", false}}));
    p.scenario("PiglinBarter targeting", event(24, PiglinBarter, {{"mItemId", 266}, {"mWasTargetingBarteringPlayer", true}}));
    p.scenario("PlayerWaxedOrUnwaxedCopper", event(25, PlayerWaxedOrUnwaxedCopper, {{"mBlockID", 595}}));
    p.scenario("CodeBuilderRuntimeAction", event(26, CodeBuilderRuntimeAction, {{"mRuntimeAction", "run"}}));
    p.scenario("CodeBuilderScoreboard", event(27, CodeBuilderScoreboard, {{"mObjectiveName", "score"}, {"mScore", -5}}));
    p.scenario("ItemUsed", event(31, ItemUsed, {{"mItemId", 1}, {"mItemAux", 0}, {"mUseMethod", 1}, {"mCount", 1}}));
    p.scenario("ItemUsed more", event(31, ItemUsed, {{"mItemId", 280}, {"mItemAux", 3}, {"mUseMethod", 4}, {"mCount", 64}}));
}
