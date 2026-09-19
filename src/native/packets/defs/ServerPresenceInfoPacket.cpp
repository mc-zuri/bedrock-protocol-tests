#include "packets/Packet.h"

namespace {
using namespace bpx::schema;
Field optString(std::string name, std::size_t at) { return optional(std::move(name), at, 0x20, string("value", 0)); }
}

BPX_PACKET("ServerPresenceInfoPacket") {
    p.since("1.26.20");
    p.shape("1.26.20", {optional("mPresenceConfiguration", 0x0, 0x40,
                                 structure("value", 0, {string("mFirst", 0x0), string("mSecond", 0x20)}))});
    p.shape("1.26.30", {optional("mPresenceConfiguration", 0x0, 0x70,
                                 structure("value", 0, {optString("mFirst", 0x0), optString("mSecond", 0x28), string("mThird", 0x50)}))});
    p.shape("1.26.40", {optional("mPresenceConfiguration", 0x0, 0x28, structure("value", 0, {optString("mRichPresenceId", 0x0)}))});

    // the reader rejects empty strings
    Object const full{{"mFirst", "lobby"}, {"mSecond", "Main Lobby"}, {"mThird", "hub"}, {"mRichPresenceId", "minigames.lobby"}};
    Object const small{{"mFirst", "a"}, {"mSecond", "b"}, {"mThird", "c"}, {"mRichPresenceId", "a"}};
    p.values("mPresenceConfiguration", {Value(), full, small});
    p.values("1.26.30", "mPresenceConfiguration", {Value(), Object{}, full, small});
}
