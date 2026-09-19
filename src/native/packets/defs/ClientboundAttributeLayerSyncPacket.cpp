#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field data(bool v26_10) {
    Field const settings =
        v26_10 ? structure("mAttributeLayerSettings", 0x28,
                           {i32("mPriority", 0x0),
                            variant("mWeight", 0x8, 0x20, {structure("float", 0, {f32("value", 0)}), structure("string", 0, {string("value", 0)})}),
                            boolean("mEnabled", 0x30), boolean("mTransitionsPaused", 0x31)})
               : structure("mAttributeLayerSettings", 0x24,
                           {i32("mPriority", 0x0), f32("mWeight", 0x4), boolean("mEnabled", 0x8), boolean("mTransitionsPaused", 0x9)});
    return variant("mData", 0x0, v26_10 ? 0x60 : 0x40,
                   {structure("UpdateAttributeLayersData", 0, {vector("mAttributeLayers", 0x0, 0x8, u64("value", 0))}),
                    structure("UpdateAttributeLayerSettingsData", 0, {string("mLayerName", 0x0), i32("mLayerDimensionId", 0x20), settings}),
                    structure("UpdateEnvironmentAttributesData", 0,
                              {string("mLayerName", 0x0), i32("mLayerDimensionId", 0x20), vector("mAttributes", 0x28, 0x8, u64("value", 0))}),
                    structure("RemoveEnvironmentAttributesData", 0,
                              {string("mLayerName", 0x0), i32("mLayerDimensionId", 0x20),
                               vector("mAttributes", 0x28, 0x20, string("value", 0))})});
}

Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }
}

BPX_PACKET("ClientboundAttributeLayerSyncPacket") {
    p.since("1.26.10");
    p.shape("1.26.10", {data(true)});
    p.shape("1.26.20", {data(false)});

    // layer and environment attribute entries (nested variants) are not built: those lists stay empty
    List values{alt(0, Object{{"mAttributeLayers", List{}}})};
    for (int dim : {0, 1, 2}) { // Overworld, Nether, TheEnd
        values.push_back(alt(2, Object{{"mLayerName", "layer"}, {"mLayerDimensionId", dim}}));
        values.push_back(alt(3, Object{{"mLayerName", "minecraft:fog"}, {"mLayerDimensionId", dim},
                                       {"mAttributes", List{"minecraft:fog_color", "minecraft:sky_color"}}}));
    }
    values.push_back(alt(3, Object{{"mLayerName", ""}, {"mAttributes", List{}}}));
    List v20 = values, v10 = values;
    for (bool enabled : {false, true})
        for (bool paused : {false, true}) {
            v20.push_back(alt(1, Object{{"mLayerName", "layer"},
                                        {"mAttributeLayerSettings", Object{{"mPriority", -5}, {"mWeight", 0.75}, {"mEnabled", enabled},
                                                                           {"mTransitionsPaused", paused}}}}));
            v10.push_back(alt(1, Object{{"mLayerName", "layer"},
                                        {"mAttributeLayerSettings",
                                         Object{{"mPriority", 100}, {"mWeight", alt(0, Object{{"value", 0.75}})}, {"mEnabled", enabled},
                                                {"mTransitionsPaused", paused}}}}));
        }
    v10.push_back(alt(1, Object{{"mLayerName", "layer"},
                                {"mAttributeLayerSettings", Object{{"mWeight", alt(1, Object{{"value", "query.time_of_day"}})}}}}));
    p.values("mData", v10);
    p.values("1.26.20", "mData", v20);
}
