#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;
Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }
}

BPX_PACKET("PlayerUpdateEntityOverridesPacket") {
    p.since("1.21.70");
    p.shape("1.21.70", {variant("mValue", 0x0, 0x4,
                                {structure("monostate", 0, {}), structure("int", 0, {i32("value", 0)}), structure("float", 0, {f32("value", 0)})}),
                        u8("mUpdateType", 0x8), i64("mId", 0x10), u32("mPropertyIndex", 0x18)});
    p.shape("1.26.40", {i64("mId", 0x0), u32("mPropertyIndex", 0x8),
                        variant("mUpdate", 0xC, 0x4,
                                {structure("ClearOverride", 0, {}), structure("RemoveOverride", 0, {}),
                                 structure("IntOverride", 0, {i32("mValue", 0)}), structure("FloatOverride", 0, {f32("mValue", 0)})})});

    p.values("mId", {1, -1, std::int64_t{-4294967295LL}});
    p.values("mPropertyIndex", {0, 1, 31});
    // ClearOverrides, RemoveOverride; the int / float only goes with SetIntOverride (2) / SetFloatOverride (3)
    p.values("mUpdateType", {0, 1});
    p.scenario("SetIntOverride", {{"mUpdateType", 2}, {"mValue", alt(1, Object{{"value", -12}})}});
    p.scenario("SetIntOverride max", {{"mUpdateType", 2}, {"mValue", alt(1, Object{{"value", 2147483647}})}});
    p.scenario("SetFloatOverride", {{"mUpdateType", 3}, {"mValue", alt(2, Object{{"value", 0.75}})}});
    p.values("mUpdate", {alt(0, Object{}), alt(1, Object{}), alt(2, Object{{"mValue", -12}}), alt(2, Object{{"mValue", 2147483647}}),
                         alt(3, Object{{"mValue", 0.75}})});
}
