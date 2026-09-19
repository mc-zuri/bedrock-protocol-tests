#include "packets/Enums.h"
#include "packets/Packet.h"

#include <algorithm>
#include <cstdint>
#include <initializer_list>

namespace {
using namespace bpx::schema;
Layout origin() {
    return {string("mCommand", 0x0), u8("mOriginType", 0x20), uuid("mOriginUuid", 0x28), string("mOriginRequestId", 0x38),
            i64("mOriginPlayerId", 0x58)};
}
}

BPX_PACKET("CommandRequestPacket") {
    namespace e = bpx::packets::enums;
    Layout l = origin();
    l.push_back(boolean("mInternalSource", 0x60));
    p.shape("1.16.201", l);
    l = origin();
    l.push_back(i32("mVersion", 0x60));
    l.push_back(boolean("mInternalSource", 0x64));
    p.shape("1.19.60", l);

    p.values("mCommand", {"", "/say hello", "/give @s minecraft:diamond 64", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    // CommandOriginType: Player 0 .. ExecuteContext 15; from 1.21.130 the reader takes only the ones a client
    // sends: Player, DevConsole, Test, AutomationPlayer
    p.values("mOriginType", e::range(0, 15));
    p.values("1.21.130", "mOriginType", {0, 3, 4, 5});
    p.values("mOriginUuid", {Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}}});
    p.values("mOriginRequestId", {"", "req-1"});
    p.values("mOriginPlayerId", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mInternalSource", {false, true});
    // CurrentCmdVersion: Initial 1 .. Latest (33 in 1.20.81, 42 in 1.21.50, 43 in 1.21.70, 44 in 1.21.90). From
    // 1.21.130 the reader takes exactly the build's enumerators: never 21, 24, 25, 27, 36 (numbers the enum skips)
    // and, above 44, the build's own newest ones (probed in each build)
    auto versions = [&](int last, std::initializer_list<int> skip) {
        List out;
        for (int v = 1; v <= last; ++v)
            if (std::find(skip.begin(), skip.end(), v) == skip.end()) out.push_back(v);
        return out;
    };
    p.values("mVersion", versions(33, {}));
    p.values("1.21.50", "mVersion", versions(42, {}));
    p.values("1.21.70", "mVersion", versions(43, {}));
    p.values("1.21.90", "mVersion", versions(44, {}));
    p.values("1.21.130", "mVersion", versions(46, {21, 24, 25, 27, 36}));
    p.values("1.26.20", "mVersion", versions(47, {21, 24, 25, 27, 36, 45}));
    p.values("1.26.30", "mVersion", versions(50, {21, 24, 25, 27, 36, 45, 46, 47, 48}));
    p.values("1.26.40", "mVersion", versions(51, {21, 24, 25, 27, 36, 45, 46, 47, 48, 49}));
    p.values("1.26.50", "mVersion", versions(52, {21, 24, 25, 27, 36, 45, 46, 47, 48, 49, 50, 51}));
}
