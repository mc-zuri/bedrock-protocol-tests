#include "packets/Packet.h"

#include <cstdint>
#include <vector>

namespace {
using namespace bpx::schema;

Field optF(char const* n, std::size_t at) { return optional(n, at, 4, f32("value", 0)); }
Field optV(char const* n, std::size_t at) { return optional(n, at, 0xC, vec3("value", 0)); }

Field extra(std::size_t at, std::size_t index) {
    return variant("mExtraDataPayload", at, index,
                   {structure("NullType", 0, {}),
                    structure("ArrowDataPayload", 0,
                              {optV("mEndLocation", 0x0), optF("mArrowHeadLength", 0x10), optF("mArrowHeadRadius", 0x18),
                               optional("mNumSegments", 0x20, 1, u8("value", 0))}),
                    structure("TextDataPayload", 0, {string("mText", 0x0)}), structure("BoxDataPayload", 0, {vec3("mBoxBound", 0x0)}),
                    structure("LineDataPayload", 0, {vec3("mEndLocation", 0x0)}),
                    structure("SphereDataPayload", 0, {u8("mNumSegments", 0x0)})});
}

Layout shapes(bool attached) {
    std::vector<Field> m{u64("mNetworkId", 0x0),
                         optional("mShapeType", 0x8, 1, u8("value", 0)),
                         optV("mLocation", 0xC),
                         optV("mRotation", 0x1C),
                         optF("mScale", 0x2C),
                         optional("mColor", 0x34, 0x10, structure("value", 0, {f32("r", 0x0), f32("g", 0x4), f32("b", 0x8), f32("a", 0xC)})),
                         optF("mTimeLeftTotalSec", 0x48)};
    if (attached) {
        m.push_back(optional("mDimensionId", 0x50, 4, i32("value", 0)));
        m.push_back(optional("mAttachedToId", 0x58, 8, u64("value", 0)));
        m.push_back(extra(0x68, 0x28));
    } else {
        m.push_back(i32("mDimensionId", 0x50));
        m.push_back(extra(0x58, 0x28));
    }
    return {vector("mShapes", 0x0, attached ? 0x98 : 0x88, structure("value", 0, m))};
}

Object xyz(double x, double y, double z) { return Object{{"x", x}, {"y", y}, {"z", z}}; }
Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }
}

BPX_PACKET("DebugDrawerPacket") {
    p.since("1.21.120");
    p.until("1.26.20");
    p.shape("1.21.120", shapes(false));
    p.shape("1.26.0", shapes(true));

    // a shape with only its id is a removal
    List each{Object{{"mNetworkId", 1}}};
    Object const arrow = alt(1, Object{{"mEndLocation", xyz(5.0, 70.0, 5.0)}, {"mArrowHeadLength", 0.5}, {"mArrowHeadRadius", 0.25},
                                       {"mNumSegments", 8}});
    struct Kind {
        int    type;
        Object extra;
    };
    // ScriptDebugShapeType: Line, Box, Sphere, Circle, Text, Arrow
    std::vector<Kind> const kinds{{0, alt(4, Object{{"mEndLocation", xyz(10.0, 64.0, 0.0)}})},
                                  {1, alt(3, Object{{"mBoxBound", xyz(1.0, 2.0, 3.0)}})},
                                  {2, alt(5, Object{{"mNumSegments", 20}})},
                                  {3, alt(5, Object{{"mNumSegments", 32}})},
                                  {4, alt(2, Object{{"mText", "\xc2\xa7" "ahello \xe2\x9c\x93"}})},
                                  {5, arrow}};
    List all;
    for (auto const& k : kinds) {
        Object s{{"mNetworkId", std::int64_t{1000} + k.type},
                 {"mShapeType", k.type},
                 {"mLocation", xyz(-12.5, 64.0, 30.25)},
                 {"mRotation", xyz(0.0, 90.0, 0.0)},
                 {"mScale", 1.5},
                 {"mColor", Object{{"r", 1.0}, {"g", 0.5}, {"b", 0.0}, {"a", 1.0}}},
                 {"mTimeLeftTotalSec", 10.0},
                 {"mDimensionId", k.type % 3},
                 {"mExtraDataPayload", k.extra}};
        each.push_back(s);
        all.push_back(s);
    }
    List values{List{}};
    for (auto const& s : each) values.push_back(List{s});
    values.push_back(all);
    p.values("mShapes", values);
    p.scenario("1.26.0", "attached to an entity",
               {{"mShapes", List{Object{{"mNetworkId", 7}, {"mShapeType", 1}, {"mAttachedToId", 42},
                                        {"mExtraDataPayload", alt(3, Object{{"mBoxBound", xyz(0.5, 0.5, 0.5)}})}}}}});
}
