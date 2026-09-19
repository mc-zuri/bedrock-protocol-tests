#include "packets/Packet.h"

BPX_PACKET("LoginPacket") {
    // a default packet has a null connection request and cannot be written
    p.captured("login");
}
