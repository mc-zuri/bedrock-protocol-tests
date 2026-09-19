#include "packets/Packet.h"

#include <cstdint>

namespace {
bpx::schema::Object xyz(double x, double y, double z) { return {{"x", x}, {"y", y}, {"z", z}}; }
}

BPX_PACKET("PlayerLocationPacket") {
    p.since("1.21.80");
    p.shape("1.21.80", {i32("mType", 0x0), i64("mId", 0x8), vec3("mPos", 0x10)});
    p.shape("1.26.40", {i64("mId", 0x0),
                        variant("mLocation", 0x8, 0x10,
                                {structure("CoordinatesLocation", 0, {i32("mType", 0x0), vec3("mPos", 0x4)}),
                                 structure("HiddenLocation", 0, {i32("mType", 0x0)})})});

    // the game leaves the location kind uninitialized, and it only writes a position for a valid one, so without a
    // baseline the scenarios that set something else export whatever the recycled packet memory happened to hold
    p.baseline("mType", 0);
    p.baseline("mLocation", Object{{"index", 0}, {"value", Object{{"mType", 0}, {"mPos", xyz(0.5, 64.0, 0.5)}}}});

    p.values("mId", {1, -1, std::int64_t{-4294967295LL}});
    p.values("mType", {0, 1}); // Coordinates, Hide
    p.values("mPos", {xyz(0.5, 64.0, 0.5), xyz(-12345.25, -59.5, 30000000.0)});
    p.values("mLocation", {Object{{"index", 0}, {"value", Object{{"mType", 0}, {"mPos", xyz(0.5, 64.0, 0.5)}}}},
                           Object{{"index", 0}, {"value", Object{{"mType", 0}, {"mPos", xyz(-12345.25, -59.5, 30000000.0)}}}},
                           Object{{"index", 1}, {"value", Object{{"mType", 1}}}}});
}
