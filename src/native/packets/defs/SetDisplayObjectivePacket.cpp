#include "packets/Packet.h"

BPX_PACKET("SetDisplayObjectivePacket") {
    p.shape("1.16.201", {
        string("mDisplaySlotName", 0x0),
        string("mObjectiveName", 0x20),
        string("mObjectiveDisplayName", 0x40),
        string("mCriteriaName", 0x60),
        u8("mSortOrder", 0x80),
    });

    p.values("mDisplaySlotName", {"sidebar", "list", "belowname"});
    p.values("mObjectiveName", {"", "kills", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mObjectiveDisplayName", {"", "Kills", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mCriteriaName", {"", "dummy"}); // the only criterion the game has
    p.values("mSortOrder", {0, 1});           // Ascending, Descending
}
