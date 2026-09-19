#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Object xyz(double x, double y, double z) { return Object{{"x", x}, {"y", y}, {"z", z}}; }

Layout since1_21_70(bool fireAt) {
    Layout l{
        string("mActorIdentifier", 0x0),
        i64("mActor", 0x20),
        u32("mEventId", 0x28),
        vec3("mPos", 0x2C),
        i32("mData", 0x38),
        boolean("mIsGlobal", 0x3C),
        boolean("mIsBaby", 0x3D),
    };
    if (fireAt) l.push_back(optional("mFireAtPosition", 0x40, 0xC, vec3("value", 0)));
    return l;
}

Field soundEvent() {
    return variant("mSoundEvent", 0x28, 0x20,
                   {structure("none", 0, {}), structure("id", 0, {u32("value", 0)}), structure("name", 0, {string("value", 0)})});
}
}

BPX_PACKET("LevelSoundEventPacket") {
    p.shape("1.16.201", {
        u32("mEventId", 0x0),
        vec3("mPos", 0x4),
        i32("mData", 0x10),
        string("mActorIdentifier", 0x18),
        boolean("mIsBaby", 0x38),
        boolean("mIsGlobal", 0x39),
    });
    p.shape("1.21.70", since1_21_70(false));
    p.shape("1.26.20", since1_21_70(true));
    p.shape("1.26.30", {
        string("mActorIdentifier", 0x0),
        i64("mActor", 0x20),
        soundEvent(),
        vec3("mPos", 0x50),
        i32("mData", 0x5C),
        boolean("mIsGlobal", 0x60),
        boolean("mIsBaby", 0x61),
        optional("mFireAtPosition", 0x64, 0xC, vec3("value", 0)),
    });

    bpx::packets::enums::levelSoundEventValues(p, "mEventId");
    bpx::packets::enums::levelSoundEventValues(p, "mSoundEvent", "1.26.30", [](int v) {
        return Value(Object{{"index", 1}, {"value", Object{{"value", v}}}});
    });
    p.scenario("mSoundEvent name", {{"mSoundEvent", Object{{"index", 2}, {"value", Object{{"value", "ambient.cave"}}}}}});
    p.scenario("mSoundEvent custom name", {{"mSoundEvent", Object{{"index", 2}, {"value", Object{{"value", "custom.pack:sound"}}}}}});
    p.scenario("mSoundEvent id", {{"mSoundEvent", Object{{"index", 1}, {"value", Object{{"value", 1}}}}}});

    p.values("mActorIdentifier", {"", "minecraft:zombie", "minecraft:player"});
    p.values("mActor", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mPos", {xyz(0.0, 64.0, 0.0), xyz(-123.5, 70.25, 456.75)});
    p.values("mData", {-1, 0, 1, 12345});
    p.values("mIsGlobal", {false, true});
    p.values("mIsBaby", {false, true});
    p.values("mFireAtPosition", {Value(), xyz(0.0, 64.0, 0.0), xyz(-123.5, 70.25, 456.75)});
}
