#include "packets/Packet.h"

BPX_PACKET("StructureTemplateDataResponsePacket") {
    p.since("1.21.60");
    p.shape("1.21.60", {
        string("mStructureName", 0x0),
        u8("mResponseType", 0x28),
    });

    p.values("mStructureName", {"", "mystructure:house", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mResponseType", {0, 1, 2}); // None, Export, Query
}
