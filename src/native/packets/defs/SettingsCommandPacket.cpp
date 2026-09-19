#include "packets/Packet.h"

BPX_PACKET("SettingsCommandPacket") {
    p.shape("1.16.201", {string("mCommand", 0x0), boolean("mSuppressOutput", 0x20)});

    p.values("mCommand", {"", "/gamerule showcoordinates true", "/time set day"});
    p.values("mSuppressOutput", {false, true});
}
