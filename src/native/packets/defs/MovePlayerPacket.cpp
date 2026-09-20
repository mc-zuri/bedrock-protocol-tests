#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Layout layout(bool optionalTeleport) {
    Layout l{u64("mPlayerID", 0x0), vec3("mPos", 0x8), vec2("mRot", 0x14), f32("mYHeadRot", 0x1C), u8("mResetPosition", 0x20),
             boolean("mOnGround", 0x21), u64("mRidingID", 0x28)};
    Field const cause = i32("mCause", 0x0), source = i32("mSourceActorType", 0x4);
    if (optionalTeleport) {
        l.push_back(optional("mTeleportData", 0x30, 0x8, structure("value", 0, {cause, source})));
        l.push_back(u64("mTick", 0x40));
    } else {
        l.push_back(structure("mTeleportData", 0x30, {cause, source}));
        l.push_back(u64("mTick", 0x38));
    }
    return l;
}
Object xyz(double x, double y, double z) { return {{"x", x}, {"y", y}, {"z", z}}; }
}

BPX_PACKET("MovePlayerPacket") {
    p.shape("1.16.201", layout(false));
    p.shape("1.26.40", layout(true));

    // the ids a server hands out are small; the protocol reads them as 32-bit varints
    p.values("mPlayerID", {1, 123456, INT_MAX});
    p.values("mPos", {xyz(0.5, 65.62, 0.5), xyz(-12345.25, -59.5, 30000000.0)});
    p.values("mRot", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", -89.9}, {"y", 179.9}}});
    p.values("mYHeadRot", {0.0, -90.0});
    p.values("mResetPosition", {0, 1, 2, 3}); // Normal, Respawn, Teleport, OnlyHeadRot
    p.values("mOnGround", {false, true});
    p.values("mRidingID", {0, 1, 123456});
    p.values("mTick", {0, 1, std::int64_t{123456789012LL}});
    // the teleport data is only sent with Teleport
    // TeleportationCause: Unknown, Projectile, ChorusFruit, Command, Behavior
    // source ActorType: none, Player 0x13F, EnderPearl 87 | Projectile 0x400000
    for (int cause = 0; cause <= 4; ++cause)
        for (std::int64_t source : {std::int64_t{0}, std::int64_t{319}, std::int64_t{4194391}})
            p.scenario("teleport", {{"mResetPosition", 2}, {"mTeleportData", Object{{"mCause", cause}, {"mSourceActorType", source}}}});
}
