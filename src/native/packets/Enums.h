#pragma once
// Sentinel enumerators (Undefined / None / Count) are deliberately left out.

#include "packets/Packet.h"

#include <functional>
#include <string_view>

namespace bpx::packets::enums {

struct Range {
    std::string_view since;
    int              max;
};
inline constexpr Range kLevelSoundEvents[] = {
    {"1.16.201", 273}, {"1.16.210", 327}, {"1.16.220", 334}, {"1.17.0", 359},   {"1.17.10", 362},  {"1.17.30", 369},
    {"1.18.0", 371},   {"1.18.11", 374},  {"1.18.30", 422},  {"1.19.1", 440},   {"1.19.10", 441},  {"1.19.30", 442},
    {"1.19.40", 447},  {"1.19.50", 457},  {"1.19.60", 461},  {"1.19.70", 465},  {"1.20.0", 469},   {"1.20.30", 476},
    {"1.20.40", 478},  {"1.20.50", 485},  {"1.21.0", 529},   {"1.21.30", 532},  {"1.21.50", 545},  {"1.21.80", 554},
    {"1.21.90", 561},  {"1.21.100", 562}, {"1.21.111", 565}, {"1.21.130", 577}, {"1.26.10", 598},  {"1.26.20", 600},
    {"1.26.30", 610},  {"1.26.40", 613},
};

inline List levelSoundEvents(std::string_view since, int max, std::function<Value(int)> const& wrap = {}) {
    bool const newer = Version::parse(since) >= Version::parse("1.21.50");
    List       out;
    for (int v = 0; v <= max; ++v) {
        if (v == 378 || (v >= 391 && v <= 425)) continue; // gaps in LevelSoundEvent
        if (newer && (v == 155 || v == 522 || v == 535)) continue;
        out.push_back(wrap ? wrap(v) : Value(v));
    }
    return out;
}

inline void levelSoundEventValues(PacketDef& p, std::string_view field, std::string_view from = "0.0.0",
                                  std::function<Value(int)> const& wrap = {}) {
    for (auto const& r : kLevelSoundEvents) {
        std::string_view since = Version::parse(r.since) < Version::parse(from) ? from : r.since;
        p.values(since, field, levelSoundEvents(r.since, r.max, wrap));
    }
}

// Ids 200 .. 299 are unused: 1.19.70 moved new packets to 300+.
inline constexpr Range kPacketIdEnd[] = {
    {"1.16.201", 163}, {"1.16.210", 164}, {"1.17.0", 167},   {"1.17.10", 169},  {"1.17.30", 172},  {"1.17.40", 175},
    {"1.18.11", 178},  {"1.18.30", 182},  {"1.19.1", 186},   {"1.19.10", 190},  {"1.19.20", 191},  {"1.19.30", 195},
    {"1.19.50", 196},  {"1.19.60", 197},  {"1.19.70", 300},  {"1.19.80", 303},  {"1.20.10", 304},  {"1.20.30", 305},
    {"1.20.50", 307},  {"1.20.61", 308},  {"1.21.0", 309},   {"1.21.2", 310},   {"1.21.20", 315},  {"1.21.30", 317},
    {"1.21.50", 320},  {"1.21.60", 322},  {"1.21.70", 325},  {"1.21.80", 327},  {"1.21.90", 328},  {"1.21.111", 329},
    {"1.21.120", 331}, {"1.21.130", 332}, {"1.26.0", 339},   {"1.26.10", 345},  {"1.26.20", 347},  {"1.26.30", 350},
    {"1.26.50", 352},
};

inline void packetIdValues(PacketDef& p, std::string_view field) {
    for (auto const& r : kPacketIdEnd) {
        List ids;
        for (int id = 1; id < r.max; ++id)
            if (id < 200 || id >= 300) ids.push_back(id);
        p.values(r.since, field, ids);
    }
}

inline List range(int from, int to) {
    List out;
    for (int v = from; v <= to; ++v) out.push_back(v);
    return out;
}

// Inventory 0, First 1 .. Last 100, Offhand 119, Armor 120, SelectionSlots 122, PlayerUIOnly 124, Registry 125
inline void containerIdValues(PacketDef& p, std::string_view field) {
    p.values(field, {0, 1, 50, 100, 119, 120, 122, 124});
    p.values("1.21.50", field, {0, 1, 50, 100, 119, 120, 122, 124, 125});
}

inline void containerTypeValues(PacketDef& p, std::string_view field) {
    p.values(field, range(-1, 33));
    p.values("1.19.1", field, range(-1, 34));
    p.values("1.20.50", field, range(-1, 36));
    p.values("1.26.50", field, range(-1, 37));
}

// Probed: the readers never accept the viewer modes (3, 4), and Spectator (6) only in 1.18.30 .. 1.19.21 and 1.19.50+.
inline void gameTypeValues(PacketDef& p, std::string_view field) {
    p.values(field, {0, 1, 2, 5});
    p.values("1.18.30", field, {0, 1, 2, 5, 6});
    p.values("1.19.30", field, {0, 1, 2, 5});
    p.values("1.19.50", field, {0, 1, 2, 5, 6});
}

// Builds before 1.20.81 are assumed to have the 1.20.81 set. 16384 is ParticlesLegacyEvent (| a particle type).
inline void levelEventValues(PacketDef& p, std::string_view field) {
    List const common{1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 1010, 1012, 1016, 1017, 1018, 1020, 1021,
                      1022, 1030, 1032, 1040, 1041, 1042, 1043, 1044, 1051, 1052, 1060, 1061, 1062, 1063, 1064, 1065, 1066,
                      1900, 1901, 1902, 1903, 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012,
                      2013, 2014, 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026, 2027, 2028, 2029,
                      2030, 2031, 2032, 2033, 2034, 2035, 2036, 2037, 2038, 2039, 3001, 3002, 3003, 3004, 3005, 3006, 3007,
                      3500, 3501, 3502, 3503, 3504, 3505, 3506, 3507, 3508, 3509, 3510, 3511, 3512, 3513, 3514, 3515, 3600,
                      3601, 3602, 3603, 3604, 3605, 3606, 3607, 3608, 3609, 3610, 3611, 3612, 3613, 3614, 3615, 3616, 9801,
                      9810, 9811, 9812, 9813, 9814, 9815, 16384};
    List before = common, after = common;
    before.push_back(3617);
    before.push_back(3618);
    for (int v : {1067, 2040, 9800, 9816}) after.push_back(v);
    p.values(field, before);
    p.values("1.21.50", field, after);
}

// Y is a varuint before 1.26.10, so negative Y only from then on.
inline void networkBlockPosValues(PacketDef& p, std::string_view field) {
    auto pos = [](int x, int y, int z) { return Value(Object{{"x", x}, {"y", y}, {"z", z}}); };
    p.values(field, {pos(0, 64, 0), pos(-123, 0, 456), pos(30000, 319, -30000)});
    p.values("1.26.10", field, {pos(0, 64, 0), pos(-123, -64, 456), pos(30000, 319, -30000)});
}

inline void playerActionTypeValues(PacketDef& p, std::string_view field) {
    struct Last {
        char const* since;
        int         last;
    };
    for (Last l : {Last{"0.0.0", 27}, Last{"1.19.1", 29}, Last{"1.19.60", 30}, Last{"1.20.10", 33}, Last{"1.20.30", 35},
                   Last{"1.20.40", 36}, Last{"1.21.50", 37}, Last{"1.26.40", 38}})
        p.values(l.since, field, range(0, l.last));
}

}
