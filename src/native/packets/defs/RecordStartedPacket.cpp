#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("RecordStartedPacket") {
    p.since("1.26.50");
    p.shape("1.26.50", {blockPos("mBlockPos", 0x0), u64("mServerSoundHandle", 0x10)});

    p.values("mBlockPos", {Object{{"x", 0}, {"y", 64}, {"z", 0}}, Object{{"x", -123}, {"y", -64}, {"z", 456}},
                           Object{{"x", 30000}, {"y", 319}, {"z", -30000}}});
    p.values("mServerSoundHandle", {0, 1, std::int64_t{123456789012LL}});
}
