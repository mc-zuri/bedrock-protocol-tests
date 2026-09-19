#pragma once

#include "packets/Packet.h"

#include <cstdint>

namespace bpx::packets::abilities {

inline Layout layout(bool verticalFlySpeed) {
    using namespace bpx::schema;
    std::vector<Field> layer{u16("mSerializedLayer", 0x0), u32("mAbilitiesSet", 0x4), u32("mAbilityValues", 0x8), f32("mFlySpeed", 0xC)};
    if (verticalFlySpeed) layer.push_back(f32("mVerticalFlySpeed", 0x10));
    layer.push_back(f32("mWalkSpeed", verticalFlySpeed ? 0x14 : 0x10));
    return {i64("mTargetPlayer", 0x0), u8("mCommandPermissions", 0x8), u8("mPlayerPermissions", 0x9),
            vector("mLayers", 0x10, verticalFlySpeed ? 0x18 : 0x14, structure("value", 0, layer))};
}

inline void values(PacketDef& p) {
    using namespace bpx::schema;
    p.values("mTargetPlayer", {1, -1, std::int64_t{-4294967295LL}});
    p.values("mCommandPermissions", {0, 1, 2, 3, 4, 5}); // CommandPermissionLevel: Any .. Internal
    p.values("mPlayerPermissions", {0, 1, 2, 3});        // PlayerPermissionLevel: Visitor, Member, Operator, Custom
    // SerializedAbilitiesLayer: CustomCache, Base, Spectator, Commands, Editor, LoadingScreen
    auto layers = [](int abilities) {
        std::int64_t const all = (std::int64_t{1} << abilities) - 1;
        List l{List{}};
        for (int layer = 0; layer <= 5; ++layer)
            l.push_back(List{Object{{"mSerializedLayer", layer}, {"mAbilitiesSet", all}, {"mAbilityValues", std::int64_t{0x2A5}},
                                    {"mFlySpeed", 0.05}, {"mVerticalFlySpeed", 1.0}, {"mWalkSpeed", 0.1}}});
        List several;
        for (int bit = 0; bit < abilities; ++bit)
            several.push_back(Object{{"mSerializedLayer", 1}, {"mAbilitiesSet", std::int64_t{1} << bit}, {"mAbilityValues", std::int64_t{1} << bit},
                                     {"mFlySpeed", 0.1}, {"mVerticalFlySpeed", 0.5}, {"mWalkSpeed", 0.2}});
        l.push_back(several);
        return l;
    };
    p.values("mLayers", layers(19)); // AbilitiesIndex count; VerticalFlySpeed (19) added in 1.21.60
    p.values("1.21.60", "mLayers", layers(20));
}

}
