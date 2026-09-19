#include "packets/Packet.h"

BPX_PACKET("SetPlayerFurnaceOptionsPacket") {
    p.since("1.26.50");
    p.shape("1.26.50", {u8("mFurnaceType", 0x0), i32("mLeftTab", 0x4), boolean("mFiltering", 0x8), i32("mLayout", 0xC)});

    p.values("mFurnaceType", {0, 1, 2, 3}); // None, Furnace, BlastFurnace, Smoker
    p.values("mLeftTab", {0, 1, 2, 3, 4, 5}); // None, RecipeFood, RecipeItems, RecipeBlocks, RecipeSearch, Inventory
    p.values("mFiltering", {false, true});
    p.values("mLayout", {0, 1, 2});           // None, InventoryOnly, Default
}
