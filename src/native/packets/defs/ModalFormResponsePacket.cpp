#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ModalFormResponsePacket") {
    p.shape("1.16.201", {
        u32("mFormId", 0x0),
        string("mJSONResponse", 0x8),
    });
    p.shape("1.19.20", {
        u32("mFormId", 0x0),
        optional("mJSONResponse", 0x8, 0x10, structure("value", 0, {i64("mValue", 0x0), u8("mType", 0x8)})),
        optional("mFormCancelReason", 0x20, 1, u8("value", 0)),
    });

    p.values("mFormId", {0, 1, 1000, std::int64_t{4294967295LL}});
    p.values("mJSONResponse", {"", "null", "[1,\"a\"]", "{\"key\":true}"});
    // Json::ValueType: nullValue 0, intValue 1, uintValue 2, realValue 3, booleanValue 5 (the others own heap data)
    p.values("1.19.20", "mJSONResponse", {Value(), Object{{"mType", 0}}, Object{{"mValue", 42}, {"mType", 1}}, Object{{"mValue", -7}, {"mType", 1}},
                               Object{{"mValue", 7}, {"mType", 2}}, Object{{"mValue", std::int64_t{0x3FF8000000000000LL}}, {"mType", 3}},
                               Object{{"mValue", 0}, {"mType", 5}}, Object{{"mValue", 1}, {"mType", 5}}});
    p.values("mFormCancelReason", {Value(), 0, 1}); // ModalFormCancelReason: UserClosed, UserBusy
}
