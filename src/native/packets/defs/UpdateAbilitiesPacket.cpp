#include "packets/Abilities.h"

BPX_PACKET("UpdateAbilitiesPacket") {
    namespace a = bpx::packets::abilities;
    p.since("1.19.10");
    p.shape("1.19.10", a::layout(false));
    p.shape("1.21.60", a::layout(true));
    a::values(p);
}
