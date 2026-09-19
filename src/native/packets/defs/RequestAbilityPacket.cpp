#include "packets/Packet.h"

#include <string>

namespace {
using namespace bpx::schema;

Object request(int ability, bool isFloat, double value) {
    Object v = isFloat ? Object{{"index", 1}, {"value", Object{{"value", value}}}}
                       : Object{{"index", 0}, {"value", Object{{"value", value != 0.0}}}};
    return Object{{"mAbility", ability}, {"mValueType", isFloat ? 2 : 1}, {"mValue", v}};
}
}

BPX_PACKET("RequestAbilityPacket") {
    p.since("1.19.1");
    // the value type sits one byte later until 1.20.10; the variant is at 0x4 with its index at 0x8 throughout
    Field const value = variant("mValue", 0x4, 0x4,
                                {structure("bool", 0, {boolean("value", 0)}), structure("float", 0, {f32("value", 0)})});
    p.shape("1.19.1", {u8("mAbility", 0x0), u8("mValueType", 0x2), value});
    p.shape("1.20.10", {u8("mAbility", 0x0), u8("mValueType", 0x1), value});

    // a default packet is Invalid / Unset, which the game writes as an ability and a value type and nothing
    // else: not a packet it ever sends, so the default carries a real request instead
    p.baseline("mAbility", 0);
    p.baseline("mValueType", 1); // bool
    p.baseline("mValue", Object{{"index", 0}, {"value", Object{{"value", false}}}});

    p.values("mAbility", {}); // only with a value (the scenarios)
    // AbilitiesIndex: Build 0 .. PrivilegedBuilder 18, VerticalFlySpeed 19 (1.21.60+); the flags as bools,
    // the speeds (FlySpeed 13, WalkSpeed 14, VerticalFlySpeed 19) as floats
    for (int a = 0; a <= 19; ++a) {
        bool const        speed = a == 13 || a == 14 || a == 19;
        std::string const n     = std::to_string(a);
        char const*       since = a == 19 ? "1.21.60" : "0.0.0";
        if (speed) {
            p.scenario(since, "ability " + n + " 0.05", request(a, true, 0.05));
            p.scenario(since, "ability " + n + " 0.1", request(a, true, 0.1));
        } else {
            p.scenario(since, "ability " + n + " false", request(a, false, 0.0));
            p.scenario(since, "ability " + n + " true", request(a, false, 1.0));
        }
    }
}
