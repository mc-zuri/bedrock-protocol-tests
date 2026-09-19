#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("LevelSoundEventPacketV1") {
    p.until("1.21.70");
    p.shape("1.16.201", {
        u8("mEventId", 0x0),
        vec3("mPos", 0x4),
        i32("mData", 0x10),
        i32("mActorType", 0x14),
        boolean("mIsBabyMob", 0x18),
        boolean("mIsGlobal", 0x19),
    });

    p.values("mEventId", bpx::packets::enums::levelSoundEvents("1.16.201", 255)); // a byte on the wire
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}}});
    p.values("mData", {-1, 0, 1, 12345});
    p.values("mActorType", {1, 319, 2849, 4876, 199456, 1116962}); // ActorType: Undefined, Player, Creeper, Pig, Zombie, Skeleton
    p.values("mIsBabyMob", {false, true});
    p.values("mIsGlobal", {false, true});
}
