#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("AgentAnimationPacket") {
    p.since("1.20.10");
    p.shape("1.20.10", {
        u8("agent_animation", 0x0),
        i64("runtime_id", 0x8),
    });
    p.shape("1.20.15", {
        i64("runtime_id", 0x8),
    });
    p.shape("1.20.30", {
        u8("agent_animation", 0x0),
        i64("runtime_id", 0x8),
    });

    p.values("agent_animation", {0, 1}); // AgentAnimation: ArmSwing, Shrug
    p.values("runtime_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
