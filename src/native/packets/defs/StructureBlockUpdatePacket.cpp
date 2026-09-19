#include "packets/Enums.h"
#include "packets/StructureSettings.h"

namespace {
using namespace bpx::schema;
namespace st = bpx::packets::structure;

Layout editor(std::size_t trigger) {
    return {blockPos("mBlockPos", 0x0), string("mStructureName", 0x10), string("mDataField", 0x30), boolean("mIncludePlayers", 0x50),
            boolean("mShowBoundingBox", 0x51), u8("mRedstoneSaveMode", 0x52), i32("mType", 0x54), boolean("mTrigger", trigger)};
}
}

BPX_PACKET("StructureBlockUpdatePacket") {
    p.shape("1.16.201", editor(0xB8));
    p.shape("1.18.30", editor(0xC0));
    {
        Layout l = editor(0xC0);
        st::settingsA(l, 0x58);
        p.shape("1.20.40", l);
        l = editor(0xC0);
        st::settingsB(l, 0x58);
        p.shape("1.21.20", l);
    }
    {
        Layout l{blockPos("mBlockPos", 0x0), string("mStructureName", 0x10),
                 optional("mFilteredStructureName", 0x30, 0x20, string("value", 0)), string("mDataField", 0x58),
                 boolean("mIncludePlayers", 0x78), boolean("mShowBoundingBox", 0x79), u8("mRedstoneSaveMode", 0x7A), i32("mType", 0x7C),
                 boolean("mTrigger", 0xE8)};
        st::settingsB(l, 0x80);
        p.shape("1.21.60", l);
    }

    bpx::packets::enums::networkBlockPosValues(p, "mBlockPos");
    p.values("mStructureName", {"", "mystructure:house"});
    p.values("mFilteredStructureName", {Value(), "mystructure:*****"});
    p.values("mDataField", {"", "data"});
    p.values("mIncludePlayers", {false, true});
    p.values("mShowBoundingBox", {false, true});
    p.values("mRedstoneSaveMode", {0, 1}); // SavesToMemory, SavesToDisk
    p.values("mType", {0, 1, 2, 3, 5});    // Data, Save, Load, Corner, Export (Invalid 4 not sent)
    p.values("mTrigger", {false, true});
    st::values(p);
    // until 1.26.40 the reader stores the waterlogged flag outside the settings, so only false reads back as written
    p.values("1.21.20", "mIsWaterLogged", {false});
    p.values("1.26.40", "mIsWaterLogged", {false, true});
}
