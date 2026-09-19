#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field containerName() {
    return structure("mFullContainerName", 0x4, {u8("mName", 0x0), optional("mDynamicId", 0x4, 4, u32("value", 0))});
}

List names(int last) {
    List out;
    for (int n = 0; n <= last; ++n) out.push_back(Object{{"mName", n}});
    out.push_back(Object{{"mName", 63 <= last ? 63 : 0}, {"mDynamicId", 0}});
    out.push_back(Object{{"mName", 63 <= last ? 63 : 0}, {"mDynamicId", 12345}});
    return out;
}
}

BPX_PACKET("InventoryContentPacket") {
    p.shape("1.16.201", {u8("mInventoryId", 0x0)});
    p.shape("1.21.20", {u8("mInventoryId", 0x0), u32("mDynamicContainerId", 0x4)});
    p.shape("1.21.30", {u8("mInventoryId", 0x0), containerName(), u32("mDynamicContainerSize", 0x10)});
    p.shape("1.21.50", {u8("mInventoryId", 0x0), containerName()});

    // ContainerID: Inventory 0, First 1 .. Last 100, Offhand 119, Armor 120, SelectionSlots 122, PlayerUIOnly 124,
    // Registry 125 (1.21.50+)
    p.values("mInventoryId", {0, 1, 50, 100, 119, 120, 122, 124});
    p.values("1.21.50", "mInventoryId", {0, 1, 50, 100, 119, 120, 122, 124, 125});
    p.values("mDynamicContainerId", {0, 1, 12345});
    p.values("mDynamicContainerSize", {0, 1, 54});
    // ContainerEnumName: AnvilInputContainer 0 .. CrafterLevelEntityContainer 62, DynamicContainer 63 (1.21.50+),
    // RecipeFood / RecipeBlocks / RecipeFurnaceItems 64 .. 66 (1.26.10+)
    p.values("1.21.30", "mFullContainerName", names(62));
    p.values("1.21.50", "mFullContainerName", names(63));
    p.values("1.26.10", "mFullContainerName", names(66));
}
