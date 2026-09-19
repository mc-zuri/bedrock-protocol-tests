#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("StructureTemplateDataExportPacket") {
    p.until("1.21.60");
    p.shape("1.16.201", {
        string("structure_name", 0x0),
        boolean("success", 0x28),
    });

    p.values("structure_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("success", {false, true});
}
