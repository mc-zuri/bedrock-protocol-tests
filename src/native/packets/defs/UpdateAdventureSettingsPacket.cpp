#include "packets/Packet.h"

BPX_PACKET("UpdateAdventureSettingsPacket") {
    p.since("1.19.10");
    p.shape("1.19.10", {boolean("noPvM", 0x0), boolean("noMvP", 0x1), boolean("immutableWorld", 0x2), boolean("showNameTags", 0x3),
                        boolean("autoJump", 0x4)});

    for (char const* f : {"noPvM", "noMvP", "immutableWorld", "showNameTags", "autoJump"}) p.values(f, {false, true});
    p.scenario("all set", {{"noPvM", true}, {"noMvP", true}, {"immutableWorld", true}, {"showNameTags", true}, {"autoJump", true}});
}
