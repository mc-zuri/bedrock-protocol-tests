#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

// pos 0: where pos / velocity / fishing live is not known
Layout older(std::size_t pos, bool items = false) {
    Field value = items ? structure("value", 0, {pointer("mItems", 0x0, 0x18, structure("value", 0, {}))}) : structure("value", 0, {});
    Layout l{pointer("mEntityData", 0x18, 0x40, value), i64("mId", 0x20), u64("mRuntimeId", 0x28)};
    if (pos) {
        l.push_back(vec3("mPos", pos));
        l.push_back(vec3("mVelocity", pos + 0xC));
        l.push_back(boolean("mIsFromFishing", pos + 0x18));
    }
    return l;
}
}

BPX_PACKET("AddItemActorPacket") {
    p.shape("1.16.201", older(0));
    p.shape("1.16.210", older(0xC0));
    p.shape("1.16.220", older(0xB0));
    p.shape("1.18.30", older(0xC8));
    p.shape("1.19.20", older(0x88));
    p.shape("1.19.30", older(0x90, true));
    p.shape("1.20.50", older(0x90));
    p.shape("1.26.40", {i64("mId", 0x18), u64("mRuntimeId", 0x20), vec3("mPos", 0x88), vec3("mVelocity", 0x94), boolean("mIsFromFishing", 0xA0)});

    // the writer dereferences mEntityData (null by default); an empty wrapper writes no data items, so only packets
    // without actor data round-trip. 1.19.30 .. 1.20.40 also dereference the wrapper's item vector.
    p.baseline("mEntityData", Object{});
    p.baseline("1.19.30", "mEntityData", Object{{"mItems", Object{}}});
    p.baseline("1.20.50", "mEntityData", Object{});
    p.values("mId", {1, -1, std::int64_t{-4294967295LL}, std::int64_t{INT64_MAX}, std::int64_t{INT64_MIN}});
    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mVelocity", {Object{{"x", 0.0}, {"y", -0.04}, {"z", 0.0}}, Object{{"x", 0.1}, {"y", 0.2}, {"z", -0.1}}});
    p.values("mIsFromFishing", {false, true});
}
