#include "packets/Packet.h"

BPX_PACKET("CodeBuilderPacket") {
    p.shape("1.16.201", {string("mURL", 0x0), boolean("mShouldOpenCodeBuilder", 0x20)});

    p.values("mURL", {"", "https://education.minecraft.net/codebuilder", "https://localhost:8080/"});
    p.values("mShouldOpenCodeBuilder", {false, true});
}
