#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Layout head(bool uniqueId) {
    Layout l{string("mName", 0x18), uuid("mUuid", 0x38)};
    if (uniqueId) l.push_back(i64("mEntityId", 0x48));
    for (Field f : {u64("mRuntimeId", 0x50), string("mPlatformChatId", 0x58), vec3("mPos", 0x78), vec3("mVelocity", 0x84),
                    vec2("mRot", 0x90), f32("mYHeadRot", 0x98)})
        l.push_back(std::move(f));
    return l;
}
// gameType / platform 0: not sent
Layout tail(Layout l, std::size_t gameType, std::size_t device, std::size_t platform) {
    if (gameType) l.push_back(i32("mPlayerGameType", gameType));
    l.push_back(string("mDeviceId", device));
    if (platform) l.push_back(i32("mBuildPlatform", platform));
    return l;
}
}

// up to 1.18.11 the writer dereferences mEntityData (null by default); an empty wrapper writes no data items
namespace {
Layout withEntityData(Layout l, std::size_t at) {
    l.push_back(pointer("mEntityData", at, 0x40, structure("value", 0, {})));
    return l;
}
}

BPX_PACKET("AddPlayerPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", withEntityData(head(true), 0x2B0));
    p.shape("1.16.210", withEntityData(tail(head(true), 0, 0x288, 0x2A8), 0x2B0));
    p.shape("1.16.220", withEntityData(tail(head(true), 0, 0x1F8, 0x218), 0x2A0));
    p.shape("1.17.0", withEntityData(tail(head(true), 0, 0x208, 0x228), 0x2B0));
    p.shape("1.18.30", tail(head(true), 0x22C, 0x208, 0x228));
    p.baseline("mEntityData", Object{});
    p.shape("1.19.1", tail(head(true), 0x2A4, 0x280, 0x2A0));
    p.shape("1.19.10", tail(head(false), 0x444, 0x420, 0x440));
    p.shape("1.19.40", tail(head(false), 0x51C, 0x4F8, 0x518));
    p.shape("1.19.70", tail(head(false), 0x554, 0x530, 0x550));
    p.shape("1.21.20", tail(head(false), 0x63C, 0x618, 0x638));
    p.shape("1.21.60", tail(head(false), 0x684, 0x660, 0x680));
    p.shape("1.26.0", tail(head(false), 0x694, 0x670, 0x690));
    p.shape("1.26.40", {
        uuid("mUuid", 0x0), string("mName", 0x10), u64("mRuntimeId", 0x30), string("mPlatformChatId", 0x38),
        vec3("mPos", 0x58), vec3("mVelocity", 0x64), vec2("mRot", 0x70), f32("mYHeadRot", 0x78),
        i32("mPlayerGameType", 0xE0), i64("mTargetPlayerRawId", 0x130), u8("mCommandPermissions", 0x138),
        u8("mPlayerPermissions", 0x139), string("mDeviceId", 0x170), i32("mBuildPlatform", 0x190),
    });

    p.baseline("mName", "Steve");
    p.values("mName", {"Steve", "Alex_123", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mUuid", {Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}}});
    p.values("mEntityId", {1, std::int64_t{-4294967295LL}});
    p.values("mRuntimeId", {1, 123456});
    p.values("mPlatformChatId", {"", "1234567890"});
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mVelocity", {Object{{"x", 0.0}, {"y", -0.0784}, {"z", 0.0}}, Object{{"x", 0.5}, {"y", 0.42}, {"z", -0.25}}});
    p.values("mRot", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 45.0}, {"y", -90.0}}});
    p.values("mYHeadRot", {0.0, 179.5});
    e::gameTypeValues(p, "mPlayerGameType");
    p.values("mTargetPlayerRawId", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mCommandPermissions", {0, 1, 2, 3, 4, 5}); // CommandPermissionLevel: Any .. Internal
    p.values("mPlayerPermissions", {0, 1, 2, 3});        // PlayerPermissionLevel: Visitor .. Custom
    p.values("mDeviceId", {"", "b4f6c1d2-35e1-4c8b-9a7f-0e1d2c3b4a59"});
    // BuildPlatform: Google 1 .. Linux 15; the reader rejects the deprecated ones (5, 10, 14 from 1.21.50; 7 from 1.26.20)
    p.values("mBuildPlatform", {1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 13, 14, 15});
    p.values("1.21.50", "mBuildPlatform", {1, 2, 3, 4, 7, 8, 9, 11, 12, 13, 15});
    p.values("1.26.20", "mBuildPlatform", {1, 2, 3, 4, 8, 9, 11, 12, 13, 15});
}
