#include "packets/Packet.h"

BPX_PACKET("NetworkSettingsPacket") {
    p.shape("1.16.201", {u16("mCompressionThreshold", 0x0)});
    p.shape("1.19.30", {u16("mCompressionThreshold", 0x0), u16("mCompressionAlgorithm", 0x4), boolean("mClientThrottleEnabled", 0x8),
                        u8("mClientThrottleThreshold", 0xC), f32("mClientThrottleScalar", 0x10)});
    p.shape("1.20.0", {u16("mCompressionThreshold", 0x0), u16("mCompressionAlgorithm", 0x2), boolean("mClientThrottleEnabled", 0x4),
                       u8("mClientThrottleThreshold", 0x8), f32("mClientThrottleScalar", 0xC)});
    p.shape("1.21.100", {u16("mCompressionThreshold", 0x0), u16("mCompressionAlgorithm", 0x2), boolean("mClientThrottleEnabled", 0x4),
                         u8("mClientThrottleThreshold", 0x5), f32("mClientThrottleScalar", 0x8)});

    p.values("mCompressionThreshold", {0, 1, 256, 65535}); // bytes: packets from this size on are compressed
    p.values("mCompressionAlgorithm", {0, 1, 65535});       // ZLib, Snappy, None
    p.values("mClientThrottleEnabled", {false, true});
    p.values("mClientThrottleThreshold", {0, 1, 10, 255});
    p.values("mClientThrottleScalar", {0.0, 0.5, 1.0});
}
