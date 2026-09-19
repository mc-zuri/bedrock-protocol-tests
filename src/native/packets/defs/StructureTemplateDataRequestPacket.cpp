#include "packets/Enums.h"
#include "packets/StructureSettings.h"

namespace {
using namespace bpx::schema;
Layout request(std::size_t op) { return {string("mStructureName", 0x0), blockPos("mStructureBlockPos", 0x20), u8("mRequestOperation", op)}; }
}

BPX_PACKET("StructureTemplateDataRequestPacket") {
    namespace st = bpx::packets::structure;
    p.shape("1.16.201", request(0x90));
    p.shape("1.18.30", request(0x98));
    {
        Layout l = request(0x98);
        st::settingsA(l, 0x30);
        p.shape("1.20.40", l);
        l = request(0x98);
        st::settingsB(l, 0x30);
        p.shape("1.21.20", l);
    }

    p.values("mStructureName", {"", "mystructure:house"});
    bpx::packets::enums::networkBlockPosValues(p, "mStructureBlockPos");
    p.values("mRequestOperation", {0, 1, 2, 3}); // None, ExportFromSaveMode, ExportFromLoadMode, QuerySavedStructure
    st::values(p);
}
