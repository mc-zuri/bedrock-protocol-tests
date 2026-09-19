#include "packets/Enums.h"
#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("CommandOutputPacket") {
    namespace e = bpx::packets::enums;
    p.shape("1.16.201", {u8("mOriginType", 0x0), uuid("mOriginUuid", 0x8), string("mOriginRequestId", 0x18), i64("mOriginPlayerId", 0x38),
                         i32("mOutputType", 0x40),
                         vector("mMessages", 0x50, 0x40,
                                structure("value", 0, {i32("mType", 0x0), string("mMessageId", 0x8), vector("mParams", 0x28, 0x20, string("value", 0))})),
                         u32("mSuccessCount", 0x68)});

    // CommandOriginType: Player 0 .. ExecuteContext 15; from 1.21.130 the reader takes only Player, DevConsole, Test,
    // AutomationPlayer
    p.values("mOriginType", e::range(0, 15));
    p.values("1.21.130", "mOriginType", {0, 3, 4, 5});
    p.values("mOriginUuid", {Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}}});
    p.values("mOriginRequestId", {"", "req-1"});
    // DevConsole, Test: the only origin types that send a player id
    for (int t : {3, 4}) p.scenario("origin player id", {{"mOriginType", t}, {"mOriginPlayerId", std::int64_t{-4294967295LL}}});
    p.values("mOutputType", {0, 1, 2, 3}); // CommandOutputType: None, LastOutput, Silent, AllOutput
    p.values("mSuccessCount", {0, 1, 1000});
    // CommandOutputMessageType: Success, Error
    p.values("mMessages", {List{}, List{Object{{"mType", 0}, {"mMessageId", "commands.give.success"}, {"mParams", List{"Diamond", "64", "Steve"}}}},
                           List{Object{{"mType", 1}, {"mMessageId", "commands.generic.unknown"}, {"mParams", List{"foo"}}},
                                Object{{"mType", 0}, {"mMessageId", "\xc2\xa7" "aDone \xe2\x9c\x93"}}}});
}
