#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

void npcDialogue(bpx::packets::PacketDef& p) {
    p.shape("1.17.10", {i64("mNpcId", 0x0), i32("mNpcDialogueActionType", 0x8), string("mDialogue", 0x10), string("mSceneName", 0x30),
                        string("mNpcName", 0x50), string("mActionJSON", 0x70)});
    p.values("mNpcId", {1, -1, std::int64_t{-4294967295LL}, std::int64_t{INT64_MAX}, std::int64_t{INT64_MIN}});
    p.values("mNpcDialogueActionType", {0, 1}); // Open, Close
    p.values("mDialogue", {"Hello, traveller!", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mSceneName", {"greeting", "scene_2"});
    p.values("mNpcName", {"Villager", "\xc2\xa7" "eTrader"});
    p.values("mActionJSON", {"[]", R"([{"button_name":"Trade","data":[{"cmd_line":"/say hi","cmd_ver":17}],"mode":0,"text":"","type":1}])"});
}
}

BPX_PACKET("NPCDialoguePacket") {
    p.until("1.20.80");
    npcDialogue(p);
}

BPX_PACKET("NpcDialoguePacket") {
    p.since("1.20.80");
    npcDialogue(p);
}
