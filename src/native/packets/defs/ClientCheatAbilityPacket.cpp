#include "packets/Abilities.h"

BPX_PACKET("ClientCheatAbilityPacket") {
    namespace a = bpx::packets::abilities;
    p.since("1.19.60");
    p.until("1.20.10");
    p.shape("1.19.60", a::layout(false));
    a::values(p);
}
