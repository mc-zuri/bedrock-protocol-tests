#include "packets/Packet.h"

BPX_PACKET("CraftingDataPacket") {
    p.shape("1.16.201", {boolean("mClearRecipes", 0x48)});
    p.shape("1.17.30", {boolean("mClearRecipes", 0x60)});
    p.shape("1.26.40", {boolean("mClearRecipes", 0x108)});

    // until 1.26.30 the reader does not keep the clear flag, so only false round-trips
    p.values("mClearRecipes", {false});
    p.values("1.26.40", "mClearRecipes", {false, true});
}
