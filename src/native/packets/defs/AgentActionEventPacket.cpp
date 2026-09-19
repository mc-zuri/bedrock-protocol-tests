#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("AgentActionEventPacket") {
    p.since("1.18.30");
    p.shape("1.18.30", {
        i32("mAction", 0x0),
        string("mRequestId", 0x8),
        structure("mResponse", 0x28, {i64("mValue", 0x0), u8("mType", 0x8)}),
    });

    List actions; // AgentActionType: Attack 1 .. Turn 18
    for (int a = 1; a <= 18; ++a) actions.push_back(a);
    p.baseline("mAction", 1);
    p.values("mAction", actions);
    p.values("mRequestId", {"", "1", "request-42"});
    // Json::ValueType: nullValue 0, intValue 1, uintValue 2, realValue 3, booleanValue 5
    p.values("mResponse", {Object{{"mType", 0}}, Object{{"mValue", 42}, {"mType", 1}}, Object{{"mValue", 7}, {"mType", 2}},
                           Object{{"mValue", std::int64_t{0x3FF8000000000000LL}}, {"mType", 3}}, Object{{"mValue", 1}, {"mType", 5}}});
}
