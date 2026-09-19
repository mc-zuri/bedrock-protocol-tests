#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ChangeDimensionPacket") {
    p.shape("1.16.201", {
        i32("mDimensionId", 0x0),
        vec3("mPos", 0x4),
        boolean("mRespawn", 0x10),
    });
    p.shape("1.21.20", {
        i32("mDimensionId", 0x0),
        vec3("mPos", 0x4),
        boolean("mRespawn", 0x10),
        optional("mLoadingScreenId", 0x14, 4, u32("value", 0)),
    });

    p.values("mDimensionId", {0, 1, 2}); // DimensionType: Overworld, Nether, TheEnd
    p.values("mPos", {Object{{"x", 0.0}, {"y", 64.0}, {"z", 0.0}}, Object{{"x", -123.5}, {"y", 70.25}, {"z", 456.75}},
                      Object{{"x", 1000.0}, {"y", -60.0}, {"z", -1000.0}}});
    p.values("mRespawn", {false, true});
    p.values("mLoadingScreenId", {Value(), 0, 1, std::int64_t{4294967295LL}});
}
