#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("SpawnParticleEffectPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {
        u8("mVanillaDimensionId", 0x0),
        i64("mActorId", 0x8),
        vec3("mPos", 0x10),
        string("mEffectName", 0x20),
    });

    p.baseline("mEffectName", "minecraft:basic_flame_particle");
    p.values("mVanillaDimensionId", {0, 1, 2}); // Overworld, Nether, TheEnd
    p.values("mActorId", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mEffectName", {"minecraft:basic_flame_particle", "minecraft:heart_particle", "custom:sparkle"});
}
