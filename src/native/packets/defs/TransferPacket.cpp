#include "packets/Packet.h"

namespace {
using namespace bpx::schema;

Field gatherings() {
    return optional("mGatheringsConfiguration", 0x28, 0xF8,
        structure("value", 0,
            {uuid("mExperienceId", 0x0), string("mExperienceName", 0x10),
             optional("mUuid2", 0x30, 0x10, uuid("value", 0)), optional("mString2", 0x48, 0x20, string("value", 0)),
             string("mCreatorId", 0x70),
             optional("mUuid3", 0x90, 0x10, uuid("value", 0)), optional("mString3", 0xA8, 0x20, string("value", 0)),
             optional("mString4", 0xD0, 0x20, string("value", 0))}));
}
}

BPX_PACKET("TransferPacket") {
    p.shape("1.16.201", {
        string("mDestination", 0x0),
        u16("mDestinationPort", 0x20),
    });
    p.shape("1.21.30", {
        string("mDestination", 0x0),
        u16("mDestinationPort", 0x20),
        boolean("mReloadWorld", 0x24),
    });
    p.shape("1.21.90", {
        string("mDestination", 0x0),
        u16("mDestinationPort", 0x20),
        boolean("mReloadWorld", 0x22),
    });
    p.shape("1.26.40", {
        string("mDestination", 0x0),
        u16("mDestinationPort", 0x20),
        boolean("mReloadWorld", 0x22),
        gatherings(),
    });

    p.baseline("mDestination", "play.example.com");
    p.baseline("mDestinationPort", 19132);

    p.values("mDestination", {"", "127.0.0.1", "play.example.com", "[2001:db8::1]"});
    p.values("mDestinationPort", {0, 19132, 19133, 65535});
    p.values("mReloadWorld", {false, true});
    Object const id{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}};
    p.values("mGatheringsConfiguration",
             {Value(),
              Object{{"mExperienceId", id}, {"mExperienceName", "Lobby"}, {"mCreatorId", "creator"}},
              Object{{"mExperienceId", id}, {"mExperienceName", "Event"}, {"mUuid2", id}, {"mString2", "venue"},
                     {"mCreatorId", "creator"}, {"mUuid3", id}, {"mString3", "scene"}, {"mString4", "extra"}}});
}
