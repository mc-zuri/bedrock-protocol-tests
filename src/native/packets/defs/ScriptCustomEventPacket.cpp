#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ScriptCustomEventPacket") {
    p.until("1.20.10");
    p.shape("1.16.201", {string("mEventName", 0x0), structure("mData", 0x20, {i64("mValue", 0x0), u8("mType", 0x8)})});

    p.values("mEventName", {"", "minecraft:custom_event", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    // Json::ValueType: nullValue 0, intValue 1, uintValue 2, realValue 3, booleanValue 5; strings / arrays / objects
    // own heap data and are not built; the reader rejects other type bytes
    p.values("mData", {Object{{"mType", 0}}, Object{{"mValue", 42}, {"mType", 1}}, Object{{"mValue", -7}, {"mType", 1}},
                       Object{{"mValue", 7}, {"mType", 2}}, Object{{"mValue", std::int64_t{0x3FF8000000000000LL}}, {"mType", 3}},
                       Object{{"mValue", 0}, {"mType", 5}}, Object{{"mValue", 1}, {"mType", 5}}});
}
