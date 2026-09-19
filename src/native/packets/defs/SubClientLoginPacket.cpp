// A default packet has a null connection request and cannot be written, so the reader gets the captured Login's
// body minus its 4-byte protocol version.
#include "packets/Captures.h"
#include "packets/Packet.h"

BPX_PACKET("SubClientLoginPacket") {
    p.captured("login", 4);
}
