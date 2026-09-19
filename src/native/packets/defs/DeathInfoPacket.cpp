#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("DeathInfoPacket") {
    p.since("1.19.10");
    p.shape("1.19.10", {
        string("death_cause_message_death_cause_attack_name", 0x0),
    });
    p.shape("1.21.100", {});
    p.shape("1.21.120", {
        string("death_cause_message_death_cause_attack_name", 0x0),
    });

    p.values("death_cause_message_death_cause_attack_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
