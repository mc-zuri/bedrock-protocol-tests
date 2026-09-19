#include "packets/Packet.h"

namespace {
using namespace bpx::schema;

Field value(bool list) {
    std::vector<Field> alts{structure("float", 0, {f32("value", 0)}), structure("bool", 0, {boolean("value", 0)}),
                            structure("string", 0, {string("value", 0)})};
    if (list) alts.push_back(structure("list", 0, {vector("value", 0, 0x20, string("value", 0))}));
    return variant("mSettingValue", 0x30, 0x20, alts);
}
Value alt(int index, Value v) { return Object{{"index", index}, {"value", Object{{"value", std::move(v)}}}}; }
}

BPX_PACKET("ServerboundPackSettingChangePacket") {
    p.since("1.21.111");
    p.shape("1.21.111", {uuid("mPackId", 0x0), string("mSettingName", 0x10), value(false)});
    p.shape("1.26.50", {uuid("mPackId", 0x0), string("mSettingName", 0x10), value(true)});

    p.values("mPackId", {Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}}});
    p.values("mSettingName", {"", "difficulty_scale", std::string(128, 's')});
    p.values("mSettingValue", {alt(0, 0.0), alt(0, 1.5), alt(0, -2.25), alt(1, false), alt(1, true), alt(2, ""), alt(2, "fast")});
    p.values("1.26.50", "mSettingValue", {alt(0, 0.0), alt(0, 1.5), alt(1, false), alt(1, true), alt(2, ""), alt(2, "fast"),
                                           alt(3, List{}), alt(3, List{"a", "bee"})});
}
