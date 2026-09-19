#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

std::vector<Field> name() { return {u8("mName", 0x0), optional("mDynamicId", 0x4, 4, u32("value", 0))}; }
}

BPX_PACKET("InventorySlotPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {u8("mInventoryId", 0x0), u32("mSlot", 0x4)});
    p.shape("1.21.20", {u8("mInventoryId", 0x0), u32("mDynamicContainerId", 0x4), u32("mSlot", 0x8)});
    p.shape("1.21.30", {u8("mInventoryId", 0x0), structure("mFullContainerName", 0x4, name()), u32("mDynamicContainerSize", 0x10),
                        u32("mSlot", 0x14)});
    p.shape("1.21.42", {u8("mInventoryId", 0x0), structure("mFullContainerName", 0x4, name()), u32("mSlot", 0x70)});
    p.shape("1.26.20", {u8("mInventoryId", 0x0), u32("mSlot", 0x4),
                        optional("mFullContainerName", 0x8, 0xC, structure("value", 0, name()))});

    e::containerIdValues(p, "mInventoryId");
    p.values("mSlot", {0, 1, 8, 35, 53});
    p.values("mDynamicContainerId", {0, 1, 12345});
    p.values("mDynamicContainerSize", {0, 1, 64});
    // ContainerEnumName: AnvilInputContainer 0 .. DynamicContainer 63 (.. RecipeFurnaceItemsContainer 66 from 1.26.10),
    // then a dynamic container with its id
    auto fullName = [&](int last, bool optional) {
        List l;
        if (optional) l.push_back(Value());
        for (int n = 0; n <= last; ++n) l.push_back(Object{{"mName", n}});
        l.push_back(Object{{"mName", 63}, {"mDynamicId", 0}});
        l.push_back(Object{{"mName", 63}, {"mDynamicId", std::int64_t{4294967295LL}}});
        return l;
    };
    p.values("1.21.30", "mFullContainerName", fullName(63, false));
    p.values("1.26.10", "mFullContainerName", fullName(66, false));
    p.values("1.26.20", "mFullContainerName", fullName(66, true));
}
