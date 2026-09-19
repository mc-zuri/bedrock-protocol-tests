#include "packets/Packet.h"

BPX_PACKET("StopSoundPacket") {
    p.shape("1.16.201", {string("mName", 0x0), boolean("mStopAll", 0x20)});
    p.shape("1.21.20", {string("mName", 0x0), boolean("mStopAll", 0x20), boolean("mStopMusicLegacy", 0x21)});

    p.values("mName", {"", "random.click", "record.cat", "custom.pack.sound"});
    p.values("mStopAll", {false, true});
    p.values("mStopMusicLegacy", {false, true});
}
