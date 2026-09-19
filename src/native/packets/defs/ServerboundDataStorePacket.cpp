#include "packets/Packet.h"

#include <string>

namespace {
using namespace bpx::schema;

Field data() {
    return variant("mData", 0x68, 0x20,
        {structure("double", 0, {f64("value", 0)}), structure("bool", 0, {boolean("value", 0)}),
         structure("string", 0, {string("value", 0)})});
}
Object number(double v) { return Object{{"index", 0}, {"value", Object{{"value", v}}}}; }
Object flag(bool v) { return Object{{"index", 1}, {"value", Object{{"value", v}}}}; }
Object text(std::string v) { return Object{{"index", 2}, {"value", Object{{"value", std::move(v)}}}}; }
}

BPX_PACKET("ServerboundDataStorePacket") {
    p.since("1.21.130");
    p.shape("1.21.130", {
        string("mDataStoreName", 0x0),
        string("mProperty", 0x20),
        u32("mPropertyUpdateCount", 0x40),
        string("mPath", 0x48),
        data(),
    });
    p.shape("1.26.0", {
        string("mDataStoreName", 0x0),
        string("mProperty", 0x20),
        u32("mPropertyUpdateCount", 0x40),
        u32("mPathUpdateCount", 0x44),
        string("mPath", 0x48),
        data(),
    });

    // protocol limits: name / property 1..1000 chars, path <= 1000, counts <= 4294967294, string data <= 5000
    p.baseline("mDataStoreName", "minecraft:form");
    p.baseline("mProperty", "value");

    p.values("mDataStoreName", {"a", "minecraft:form", std::string(1000, 'n')});
    p.values("mProperty", {"a", "value", "\xc2\xa7" "aunicode \xe2\x9c\x93", std::string(1000, 'p')});
    p.values("mPropertyUpdateCount", {0, 1, 1000, std::int64_t{4294967294LL}});
    p.values("mPathUpdateCount", {0, 1, 1000, std::int64_t{4294967294LL}});
    p.values("mPath", {"", "items[0].label", std::string(1000, 'q')});
    p.values("mData", {number(0.0), number(1.5), number(-2.25), number(1e300), flag(false), flag(true), text(""), text("hello"),
                       text("\xc2\xa7" "aunicode \xe2\x9c\x93"), text(std::string(5000, 's'))});
}
