#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Field marker() {
    return optional("mDebugMarkerData", 0x8, 0x48,
        structure("value", 0,
            {string("text", 0x0), vec3("position", 0x20),
             structure("color", 0x2C, {f32("r", 0x0), f32("g", 0x4), f32("b", 0x8), f32("a", 0xC)}),
             u64("durationMS", 0x40)}));
}

Object cube(std::string text, double x, double y, double z, double r, double g, double b, double a, std::int64_t ms) {
    return Object{{"mType", 2},
                  {"mDebugMarkerData",
                   Object{{"text", std::move(text)},
                          {"position", Object{{"x", x}, {"y", y}, {"z", z}}},
                          {"color", Object{{"r", r}, {"g", g}, {"b", b}, {"a", a}}},
                          {"durationMS", ms}}}};
}
}

BPX_PACKET("ClientboundDebugRendererPacket") {
    p.since("1.16.210");
    p.shape("1.16.210", {u32("mType", 0x0), marker()});
    p.shape("1.21.130", {u8("mType", 0x0), marker()});

    // Type: Invalid 0 (rejected before 1.21.130), ClearDebugMarkers 1; AddDebugMarkerCube 2 writes the marker, which
    // must then be engaged (else std::bad_optional_access): scenarios below
    p.baseline("mType", 1);
    p.values("mType", {1});
    p.values("1.21.130", "mType", {0, 1});

    p.scenario("cube", cube("", 0.0, 64.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0));
    p.scenario("cube text", cube("hello", -123.5, 70.25, 456.75, 1.0, 0.0, 0.0, 0.5, 1000));
    p.scenario("cube unicode", cube("\xc2\xa7" "aunicode \xe2\x9c\x93", 1000.0, -60.0, -1000.0, 0.0, 0.5, 1.0, 1.0, 60000));
    p.scenario("cube long", cube(std::string(300, 'a'), 0.5, 319.0, 0.5, 0.25, 0.75, 0.0, 0.0, 3600000));
}
