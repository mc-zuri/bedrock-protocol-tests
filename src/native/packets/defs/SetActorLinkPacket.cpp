#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("SetActorLinkPacket") {
    Layout base{u8("mType", 0x0), i64("mA", 0x8), i64("mB", 0x10), boolean("mImmediate", 0x18), boolean("mPassengerInitiated", 0x19)};
    p.shape("1.16.201", base);
    base.push_back(f32("mVehicleAngularVelocity", 0x1C));
    p.shape("1.21.20", base);

    p.values("mType", {0, 1, 2}); // ActorLinkType: None, Riding, Passenger
    p.values("mA", {1, std::int64_t{-4294967295LL}});
    p.values("mB", {2, std::int64_t{-8589934591LL}});
    p.values("mImmediate", {false, true});
    p.values("mPassengerInitiated", {false, true});
    p.values("mVehicleAngularVelocity", {0.0, 1.5, -2.25});
}
