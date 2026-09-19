#include "packets/Enums.h"
#include "packets/Packet.h"

BPX_PACKET("PacketViolationWarningPacket") {
    p.shape("1.16.201", {
        i32("mViolationType", 0x0),
        i32("mViolationSeverity", 0x4),
        i32("mViolatingPacketId", 0x8),
        string("mViolationContext", 0x10),
    });

    p.values("mViolationType", {0});           // PacketMalformed (Unknown -1 is not sent)
    p.values("mViolationSeverity", {0, 1, 2}); // Warning, FinalWarning, TerminatingConnection
    bpx::packets::enums::packetIdValues(p, "mViolatingPacketId");
    p.values("mViolationContext", {"", "Malformed packet", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
