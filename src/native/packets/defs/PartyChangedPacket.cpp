#include "packets/Packet.h"

BPX_PACKET("PartyChangedPacket") {
    p.since("1.26.10");
    p.shape("1.26.10", {
        optional("mPartyId", 0x0, 0x20, string("value", 0)),
    });
    p.shape("1.26.20", {
        optional("mPartyInfo", 0x0, 0x28, structure("value", 0, {string("partyId", 0x0), boolean("isLeader", 0x20)})),
    });

    p.values("mPartyId", {Value(), "", "9f2c1a3e-5b7d-4e8f-a1b2-c3d4e5f60718"});
    p.values("mPartyInfo", {Value(),
                            Object{{"partyId", ""}, {"isLeader", false}},
                            Object{{"partyId", "9f2c1a3e-5b7d-4e8f-a1b2-c3d4e5f60718"}, {"isLeader", false}},
                            Object{{"partyId", "9f2c1a3e-5b7d-4e8f-a1b2-c3d4e5f60718"}, {"isLeader", true}}});
}
