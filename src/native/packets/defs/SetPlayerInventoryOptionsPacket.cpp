#include "packets/Packet.h"

BPX_PACKET("SetPlayerInventoryOptionsPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {i32("mLeftInventoryTab", 0x0), i32("mRightInventoryTab", 0x4), boolean("mFiltering", 0x8),
                        i32("mLayoutInv", 0xC), i32("mLayoutCraft", 0x10)});

    p.values("mLeftInventoryTab", {0, 1, 2, 3, 4, 5, 6}); // None, RecipeConstruction .. Survival
    p.values("mRightInventoryTab", {0, 1, 2, 3});         // None, FullScreen, Crafting, Armor
    p.values("mFiltering", {false, true});
    p.values("mLayoutInv", {0, 1, 2, 3});   // None, InventoryOnly, Default, RecipeBookOnly
    p.values("mLayoutCraft", {0, 1, 2, 3});
}
