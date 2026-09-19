#include "packets/Enums.h"
#include "packets/Packet.h"

namespace {
bpx::schema::Object messages(char const* m, char const* f) {
    return {{"index", 0}, {"value", bpx::schema::Object{{"mMessage", m}, {"mFilteredMessage", f}}}};
}
}

BPX_PACKET("DisconnectPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {boolean("mSkipMessage", 0x0), string("mMessage", 0x8)});
    p.shape("1.20.40", {boolean("mSkipMessage", 0x0), string("mMessage", 0x8), i32("mReason", 0x28)});
    p.shape("1.21.20", {boolean("mSkipMessage", 0x0), string("mMessage", 0x8), optional("mFilteredMessage", 0x28, 0x20, string("value", 0)),
                        i32("mReason", 0x50)});
    p.shape("1.26.20", {i32("mReason", 0x0),
                        variant("mMessages", 0x8, 0x40,
                                {structure("DisconnectPacketMessages", 0, {string("mMessage", 0x0), string("mFilteredMessage", 0x20)}),
                                 structure("NullType", 0, {})})});

    p.values("mSkipMessage", {false, true});
    p.values("mMessage", {"", "disconnectionScreen.serverFull", "\xc2\xa7" "cKicked \xe2\x9c\x93"});
    p.values("mFilteredMessage", {Value(), "", "K*****"});
    // index 1 (NullType): the disconnection screen is skipped
    p.values("mMessages", {messages("", ""), messages("disconnectionScreen.serverFull", ""), messages("\xc2\xa7" "cKicked \xe2\x9c\x93", "K*****"),
                           Object{{"index", 1}, {"value", Object{}}}});
    struct Last {
        char const* since;
        int         last;
    };
    for (Last l : {Last{"1.20.40", 115}, Last{"1.21.50", 117}, Last{"1.21.80", 118}, Last{"1.21.90", 121}, Last{"1.21.100", 122},
                   Last{"1.21.120", 124}, Last{"1.21.130", 135}, Last{"1.26.20", 139}, Last{"1.26.30", 141}, Last{"1.26.40", 147},
                   Last{"1.26.50", 149}})
        p.values(l.since, "mReason", e::range(0, l.last));
}
