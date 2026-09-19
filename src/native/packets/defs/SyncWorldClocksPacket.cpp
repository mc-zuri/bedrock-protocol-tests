#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field marker() {
    return structure("value", 0, {u64("mId", 0x0), string("mName", 0x8), i32("mTime", 0x28), optional("mPeriod", 0x2C, 4, i32("value", 0))});
}
Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }
Object markerValue(std::int64_t id, char const* name, int time, Value period) {
    return Object{{"mId", id}, {"mName", name}, {"mTime", time}, {"mPeriod", std::move(period)}};
}
}

BPX_PACKET("SyncWorldClocksPacket") {
    p.since("1.26.10");
    p.shape("1.26.10",
            {variant("mData", 0x0, 0x20,
                     {structure("SyncStateData", 0,
                                {vector("mClockData", 0x0, 0x10,
                                        structure("value", 0, {u64("mClockId", 0x0), i32("mTime", 0x8), boolean("mIsPaused", 0xC)}))}),
                      structure("InitializeRegistryData", 0,
                                {vector("mClockData", 0x0, 0x48,
                                        structure("value", 0,
                                                  {u64("mId", 0x0), string("mName", 0x8), i32("mTime", 0x28), boolean("mIsPaused", 0x2C),
                                                   vector("mTimeMarkers", 0x30, 0x38, marker())}))}),
                      structure("AddTimeMarkerData", 0, {u64("mClockId", 0x0), vector("mTimeMarkers", 0x8, 0x38, marker())}),
                      structure("RemoveTimeMarkerData", 0, {u64("mClockId", 0x0), vector("mTimeMarkerIds", 0x8, 0x8, u64("value", 0))})})});

    List const markers{markerValue(1, "minecraft:noon", 6000, Value()), markerValue(2, "custom:every_hour", 0, 1000)};
    p.values("mData",
             {alt(0, Object{{"mClockData", List{}}}),
              alt(0, Object{{"mClockData", List{Object{{"mClockId", 0}, {"mTime", 1000}, {"mIsPaused", false}},
                                                Object{{"mClockId", 7}, {"mTime", -5}, {"mIsPaused", true}}}}}),
              alt(1, Object{{"mClockData", List{}}}),
              alt(1, Object{{"mClockData", List{Object{{"mId", 0}, {"mName", "minecraft:overworld"}, {"mTime", 24000}, {"mIsPaused", false},
                                                       {"mTimeMarkers", markers}},
                                                Object{{"mId", 3}, {"mName", ""}, {"mTime", 0}, {"mIsPaused", true}}}}}),
              alt(2, Object{{"mClockId", 0}, {"mTimeMarkers", markers}}),
              alt(3, Object{{"mClockId", 0}, {"mTimeMarkerIds", List{1, 2, std::int64_t{0x7FFFFFFFFFFFFFFFLL}}}}),
              alt(3, Object{{"mClockId", 5}, {"mTimeMarkerIds", List{}}})});
}
