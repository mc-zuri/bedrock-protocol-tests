#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("NpcRequestPacket") {
    Layout base{u64("mId", 0x0), u8("mType", 0x8), string("mActions", 0x10), u8("mActionIndex", 0x30)};
    p.shape("1.16.201", base);
    base.push_back(string("mSceneName", 0x38));
    p.shape("1.17.10", base);

    p.values("mId", {1, 123456, std::int64_t{1000000000000LL}});
    // SetActions, ExecuteAction, ExecuteClosingCommands, SetName, SetSkin, SetInteractText, + ExecuteOpeningCommands
    p.values("mType", {0, 1, 2, 3, 4, 5});
    p.values("1.17.10", "mType", {0, 1, 2, 3, 4, 5, 6});
    p.values("mActions", {"", "[{\"button_name\":\"Go\",\"data\":[],\"mode\":0,\"text\":\"say hi\",\"type\":1}]"});
    p.values("mActionIndex", {0, 1, 255});
    p.values("mSceneName", {"", "intro_scene"});
}
