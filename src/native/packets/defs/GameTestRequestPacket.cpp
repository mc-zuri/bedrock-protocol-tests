#include "packets/Packet.h"

BPX_PACKET("GameTestRequestPacket") {
    p.since("1.19.30");
    p.shape("1.19.30", {
        string("mTestName", 0x0),
        blockPos("mPos", 0x20),
        boolean("mStopOnFailure", 0x2C),
        i32("mRepeatCount", 0x30),
        i32("mTestsPerRow", 0x34),
        i32("mMaxTestsPerBatch", 0x38),
        u8("mRotation", 0x58),
    });

    p.values("mTestName", {"", "minecraft:simple_test", "custom:tests/door"});
    p.values("mPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                      Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mStopOnFailure", {false, true});
    p.values("mRepeatCount", {0, 1, 10});
    p.values("mTestsPerRow", {0, 1, 8});
    p.values("mMaxTestsPerBatch", {0, 1, 64});
    p.values("mRotation", {0, 1, 2, 3}); // Rotation: None, Rotate90 (Ccw), Rotate180 (Opp), Rotate270 (Cw)
}
