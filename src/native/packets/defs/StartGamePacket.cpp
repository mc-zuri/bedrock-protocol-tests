#include "packets/Enums.h"
#include "packets/Packet.h"

#include <climits>
#include <cstdint>

namespace {
using namespace bpx::schema;

Layout own(Layout l, std::size_t b) {
    for (Field f : {i64("mEntityId", b), i64("mRuntimeId", b + 0x8), i32("mEntityGameType", b + 0x10), f32("mPosX", b + 0x14),
                    f32("mPosY", b + 0x18), f32("mPosZ", b + 0x1C), f32("mRotX", b + 0x20), f32("mRotY", b + 0x24),
                    string("mLevelId", b + 0x28), string("mLevelName", b + 0x48)})
        l.push_back(std::move(f));
    return l;
}
Layout early(std::size_t b) {
    return own({i32("mSeed", 0x0), i32("mGameType", 0x4), i32("mGameDifficulty", 0x8),
                i32("mGenerator", 0x10)}, b);
}
Layout middle(std::size_t b) {
    return own({i64("mSeed", 0x0), i32("mGameType", 0x8), i32("mGameDifficulty", 0xC),
                i32("mGenerator", 0x14)}, b);
}
Layout late(std::size_t b) {
    return own({i64("mSeed", 0x0), i32("mGameType", 0x8), boolean("mIsHardcore", 0xC),
                i32("mGameDifficulty", 0x10), i32("mGenerator", 0x18)}, b);
}
}

BPX_PACKET("StartGamePacket") {
    // up to 1.26.30 only the LevelSettings head and the packet's own fields; the rest of LevelSettings keeps its defaults
    p.shape("1.16.201", early(0x3C0));
    p.shape("1.17.0", early(0x3E8));
    p.shape("1.17.30", early(0x4B8));
    p.shape("1.18.11", early(0x4D8));
    p.shape("1.18.30", middle(0x4E0));
    p.shape("1.19.1", middle(0x390));
    p.shape("1.19.20", middle(0x3A0));
    p.shape("1.20.30", middle(0x3A8));
    p.shape("1.20.61", middle(0x440));
    p.shape("1.20.71", middle(0x4C0));
    p.shape("1.20.80", late(0x4E0));
    p.shape("1.21.0", late(0x540));
    p.shape("1.21.20", late(0x568));
    p.shape("1.21.42", late(0x560));
    p.shape("1.21.60", late(0x548));
    p.shape("1.21.70", late(0x568));
    p.shape("1.21.80", late(0x580));
    p.shape("1.21.90", late(0x5A0));
    p.shape("1.26.0", late(0x538));
    p.shape("1.26.30", late(0x540));
    p.shape("1.26.40", {
        i64("mSeed", 0x0),
        i32("mGameType", 0x8),
        boolean("mIsHardcore", 0xC),
        i32("mGameDifficulty", 0x10),
        i32("mGenerator", 0x18),
        boolean("mNetherType", 0x31),
        i16("mSpawnBiomeType", 0x38),
        i32("mSpawnDimension", 0x60),
        boolean("mAchievementsDisabled", 0x68),
        i32("mEditorWorldType", 0x6C),
        boolean("mAllowAnonymousBlockDropsInEditorWorlds", 0x70),
        i32("mServerEditorConnectionPolicy", 0x74),
        boolean("mIsCreatedInEditor", 0x78),
        boolean("mIsExportedFromEditor", 0x79),
        i32("mDayCycleStopTime", 0x7C),
        i32("mEducationEditionOffer", 0x80),
        boolean("mEducationFeaturesEnabled", 0x84),
        boolean("mImmutableWorld", 0x85),
        f32("mRainLevel", 0x88),
        f32("mLightningLevel", 0x8C),
        boolean("mConfirmedPlatformLockedContent", 0x90),
        boolean("mMultiplayerGameIntent", 0x91),
        boolean("mLANBroadcastIntent", 0x92),
        i32("mXBLBroadcastIntent", 0x94),
        i32("mPlatformBroadcastIntent", 0x98),
        u8("mDaylightCycle", 0x9C),
        boolean("mDisablePlayerInteractions", 0x9D),
        boolean("mCheatsEnabled", 0x9E),
        boolean("mAdventureModeOverridesEnabled", 0x9F),
        boolean("mCommandsEnabled", 0xA0),
        boolean("mTexturePacksRequired", 0xA1),
        boolean("mHasLockedBehaviorPack", 0xA2),
        boolean("mHasLockedResourcePack", 0xA3),
        boolean("mIsFromLockedTemplate", 0xA4),
        boolean("mIsRandomSeedAllowed", 0xA5),
        boolean("mUseMsaGamertagsOnly", 0xA6),
        boolean("mBonusChestEnabled", 0xA8),
        boolean("mStartWithMapEnabled", 0xA9),
        i32("mServerChunkTickRange", 0xAC),
        boolean("mIsFromWorldTemplate", 0xB0),
        boolean("mIsWorldTemplateOptionLocked", 0xB1),
        boolean("mSpawnV1Villagers", 0xB2),
        boolean("mPersonaDisabled", 0xB3),
        boolean("mCustomSkinsDisabled", 0xB4),
        boolean("mEmoteChatMuted", 0xB5),
        i32("mLimitedWorldWidth", 0xB8),
        i32("mLimitedWorldDepth", 0xBC),
        u8("mPlayerPermissions", 0xC1),
        i32("mDefaultSpawnX", 0xD8),
        i32("mDefaultSpawnY", 0xDC),
        i32("mDefaultSpawnZ", 0xE0),
        boolean("mExperimentsEverToggled", 0x218),
        string("mEducationProductID", 0x240),
        string("mEduSharedUriLinkUri", 0x2E0),
        string("mEduSharedUriButtonName", 0x300),
        u8("mChatRestrictionLevel", 0x320),
        optional("mOverrideForceExperimentalGameplayFlag", 0x470, 1, boolean("value", 0)),
        i64("mEntityId", 0x540),
        i64("mRuntimeId", 0x548),
        i32("mEntityGameType", 0x550),
        f32("mPosX", 0x554),
        f32("mPosY", 0x558),
        f32("mPosZ", 0x55C),
        f32("mRotX", 0x560),
        f32("mRotY", 0x564),
        string("mLevelId", 0x568),
        string("mLevelName", 0x588),
        i64("mWorldTemplateIdHigh", 0x5C0),
        i64("mWorldTemplateIdLow", 0x5C8),
        boolean("mIsTrial", 0x5D0),
        i32("mRewindHistorySize", 0x5D4),
        boolean("mServerAuthBlockBreaking", 0x5D8),
        i64("mLevelCurrentTime", 0x5E0),
        i32("mEnchantmentSeed", 0x5E8),
        string("mMultiplayerCorrelationId", 0x5F0),
        boolean("mEnableItemStackNetManager", 0x610),
        string("mServerVersion", 0x618),
        i64("mServerBlockTypeRegistryChecksum", 0x650),
    });

    namespace e = bpx::packets::enums;
    p.values("mSeed", {0, 12345, -1, std::int64_t{-4172144997902289642LL}});
    p.values("1.16.201", "mSeed", {0, 12345, -1, 2147483647});
    p.values("1.18.30", "mSeed", {0, 12345, -1, std::int64_t{-4172144997902289642LL}});
    e::gameTypeValues(p, "mGameType");
    p.values("mIsHardcore", {false, true});
    p.values("mGameDifficulty", {0, 1, 2, 3});   // Peaceful .. Hard
    p.values("mGenerator", {0, 1, 2, 3, 4, 5}); // Legacy, Overworld, Flat, Nether, TheEnd, Void
    p.values("mNetherType", {false, true});        // Normal, Flat
    p.values("mSpawnBiomeType", {0, 1}); // Default, UserDefined
    p.values("mSpawnDimension", {0, 1, 2});
    p.values("mAchievementsDisabled", {false, true});
    // Local .. Progress; the reader rejects the later Editor::WorldType values (up to 8)
    p.values("mEditorWorldType", {0, 1, 2, 3});
    p.values("mAllowAnonymousBlockDropsInEditorWorlds", {false, true});
    p.values("mServerEditorConnectionPolicy", {0, 1, 2, 3}); // MatchWorldType, EditorOnly, VanillaOnly, Mixed
    p.values("mIsCreatedInEditor", {false, true});
    p.values("mIsExportedFromEditor", {false, true});
    p.values("mDayCycleStopTime", {-1, 0, 6000, 18000}); // ticks (-1: the cycle runs)
    p.values("mEducationEditionOffer", {0, 1, 2});        // None, RestOfWorld, ChinaDeprecated
    p.values("mEducationFeaturesEnabled", {false, true});
    p.values("mRainLevel", {0.0, 0.5, 1.0});
    p.values("mLightningLevel", {0.0, 0.5, 1.0});
    p.values("mImmutableWorld", {false, true});
    p.values("mDaylightCycle", {0, 1, 2}); // Normal, AlwaysDay, LockTime
    p.values("mDisablePlayerInteractions", {false, true});
    p.values("mCheatsEnabled", {false, true});
    p.values("mAdventureModeOverridesEnabled", {false, true});
    p.values("mIsRandomSeedAllowed", {false, true});
    p.values("mConfirmedPlatformLockedContent", {false, true});
    p.values("mMultiplayerGameIntent", {false, true});
    p.values("mLANBroadcastIntent", {false, true});
    p.values("mXBLBroadcastIntent", {0, 1, 2, 3, 4});  // NoMultiPlay .. Public
    p.values("mPlatformBroadcastIntent", {0, 1, 2, 3, 4});
    p.values("mChatRestrictionLevel", {0, 1, 2}); // None, Dropped, Disabled
    p.values("mCommandsEnabled", {false, true});
    p.values("mTexturePacksRequired", {false, true});
    p.values("mHasLockedBehaviorPack", {false, true});
    p.values("mHasLockedResourcePack", {false, true});
    p.values("mIsFromLockedTemplate", {false, true});
    p.values("mUseMsaGamertagsOnly", {false, true});
    p.values("mBonusChestEnabled", {false, true});
    p.values("mStartWithMapEnabled", {false, true});
    p.values("mServerChunkTickRange", {0, 4, 8, 12});
    p.values("mIsFromWorldTemplate", {false, true});
    p.values("mIsWorldTemplateOptionLocked", {false, true});
    p.values("mSpawnV1Villagers", {false, true});
    p.values("mPersonaDisabled", {false, true});
    p.values("mCustomSkinsDisabled", {false, true});
    p.values("mEmoteChatMuted", {false, true});
    p.values("mLimitedWorldWidth", {0, 16, 64});
    p.values("mLimitedWorldDepth", {0, 16, 64});
    p.values("mPlayerPermissions", {0, 1, 2, 3}); // Visitor .. Custom
    p.values("mDefaultSpawnX", {0, -123, 30000});
    p.values("mDefaultSpawnY", {64, 0, 319, 32767}); // 32767: "none set"
    p.values("mDefaultSpawnZ", {0, 456, -30000});
    p.values("mExperimentsEverToggled", {false, true});
    p.values("mEducationProductID", {"", "product-1"});
    p.values("mEduSharedUriLinkUri", {"", "https://example.com/"});
    p.values("mEduSharedUriButtonName", {"", "Open"});
    p.values("mOverrideForceExperimentalGameplayFlag", {Value(), false, true});
    p.values("mEntityId", {1, std::int64_t{-4294967295LL}});
    p.values("mRuntimeId", {1, 123456});
    e::gameTypeValues(p, "mEntityGameType");
    p.values("mPosX", {0.0, -123.5, 1000.0});
    p.values("mPosY", {64.0, -60.0, 319.0});
    p.values("mPosZ", {0.0, 456.75, -1000.0});
    p.values("mRotX", {0.0, 45.0, -90.0});
    p.values("mRotY", {0.0, 179.5});
    p.values("mLevelId", {"", "d2b0AAAAAAA="});
    p.values("mLevelName", {"", "My World", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mWorldTemplateIdHigh", {0, std::int64_t{0x0123456789ABCDEFLL}});
    p.values("mWorldTemplateIdLow", {0, std::int64_t{0x0FEDCBA987654321LL}});
    p.values("mIsTrial", {false, true});
    p.values("mRewindHistorySize", {0, 20, 40});
    p.values("mServerAuthBlockBreaking", {false, true});
    p.values("mLevelCurrentTime", {0, 6000, std::int64_t{123456789LL}});
    p.values("mEnchantmentSeed", {0, 1, -123456});
    p.values("mMultiplayerCorrelationId", {"", "1a2b3c4d-5e6f-4a7b-8c9d-0e1f2a3b4c5d"});
    p.values("mEnableItemStackNetManager", {false, true});
    p.values("mServerVersion", {"", "1.26.40"});
    p.values("mServerBlockTypeRegistryChecksum", {0, std::int64_t{0x1234567890ABCDEFLL}});
}
