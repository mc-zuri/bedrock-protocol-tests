#include "packets/Packet.h"

BPX_PACKET("UpdateSoftEnumPacket") {
    p.shape("1.16.201", {string("mEnumName", 0x0), vector("mValues", 0x20, 0x20, string("value", 0)), u8("mType", 0x38)});
    p.shape("1.21.100", {u8("mType", 0x0), string("mEnumName", 0x8), vector("mValues", 0x28, 0x20, string("value", 0))});

    p.values("mEnumName", {"", "ScoreboardObjectives", "custom:modes"});
    p.values("mValues", {List{}, List{"kills"}, List{"easy", "normal", "hard", "\xc2\xa7" "aunicode \xe2\x9c\x93"}});
    p.values("mType", {0, 1, 2}); // Add, Remove, Replace
}
