#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("MotionPredictionHintsPacket") {
    p.shape("1.16.201", {u64("mRuntimeId", 0x0), vec3("mMotion", 0x8), boolean("mOnGround", 0x14)});

    p.values("mRuntimeId", {1, 123456, std::int64_t{1000000000000LL}});
    p.values("mMotion", {Object{{"x", 0.0}, {"y", -0.0784}, {"z", 0.0}}, Object{{"x", 0.5}, {"y", 0.42}, {"z", -0.25}}});
    p.values("mOnGround", {false, true});
}
