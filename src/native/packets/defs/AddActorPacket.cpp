#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Layout older(std::size_t entityData, bool items, bool bodyRotation, std::size_t type) {
    Layout l{vec3("mPos", 0x18), vec3("mVelocity", 0x24), vec2("mRot", 0x30), f32("mYHeadRotation", 0x38)};
    if (bodyRotation) l.push_back(f32("mYBodyRotation", 0x3C));
    l.push_back(i64("mEntityId", 0x40));
    l.push_back(u64("mRuntimeId", 0x48));
    l.push_back(string("mActorType", type + 0x88));
    Field value = items ? structure("value", 0, {pointer("mItems", 0x0, 0x18, structure("value", 0, {}))}) : structure("value", 0, {});
    l.push_back(pointer("mEntityData", entityData, 0x40, value));
    return l;
}
}

BPX_PACKET("AddActorPacket") {
    p.shape("1.16.201", older(0x50, false, false, 0x70));
    p.shape("1.19.10", older(0x50, false, true, 0x70));
    p.shape("1.19.30", older(0x50, true, true, 0x70));
    p.shape("1.20.50", older(0x178, false, true, 0x68));
    p.shape("1.21.60", older(0x180, false, true, 0x68));
    p.shape("1.26.40", {i64("mEntityId", 0x0), u64("mRuntimeId", 0x8), string("mActorType", 0x10), vec3("mPos", 0x30),
                        vec3("mVelocity", 0x3C), vec2("mRot", 0x48), f32("mYHeadRotation", 0x50), f32("mYBodyRotation", 0x54)});

    // the writer dereferences mEntityData (null by default); an empty wrapper writes no data items, so only packets
    // without actor data round-trip. 1.19.30 .. 1.20.40 also dereference the wrapper's item vector.
    p.baseline("mEntityData", Object{});
    p.baseline("1.19.30", "mEntityData", Object{{"mItems", Object{}}});
    p.baseline("1.20.50", "mEntityData", Object{});
    // the reader prefixes a type without a namespace with "minecraft:"
    p.baseline("mActorType", "minecraft:pig");
    p.values("mEntityId", {1, -1, std::int64_t{-4294967295LL}, std::int64_t{INT64_MAX}, std::int64_t{INT64_MIN}});
    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mActorType", {"minecraft:pig", "minecraft:zombie", "minecraft:armor_stand"});
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mVelocity", {Object{{"x", 0.0}, {"y", -0.0784}, {"z", 0.0}}, Object{{"x", 0.5}, {"y", 0.42}, {"z", -0.25}}});
    p.values("mRot", {Object{{"x", 0.0}, {"y", 0.0}}, Object{{"x", 45.0}, {"y", -90.0}}});
    p.values("mYHeadRotation", {0.0, 179.5, -90.0});
    p.values("mYBodyRotation", {0.0, 179.5, -90.0});
}
