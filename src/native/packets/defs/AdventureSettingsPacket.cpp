#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("AdventureSettingsPacket") {
    p.until("1.19.30");
    p.shape("1.16.201", {u32("mFlags", 0x0), u8("mCommandPermissionLevel", 0x4), u32("mActionPermissions", 0x8),
                         u8("mPlayerPermissionLevel", 0xC), i64("mUniqueId", 0x10), u32("mCustomStoredPermissions", 0x1C)});

    List flags{0};
    // WorldImmutable, NoPvM, NoMvP, ShowNameTags, AutoJump, AllowFlight, NoClip, WorldBuilder, Flying, Muted
    for (int b : {1, 2, 4, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400}) flags.push_back(b);
    flags.push_back(0x7F7);
    p.values("mFlags", flags);
    p.values("mCommandPermissionLevel", {0, 1, 2, 3, 4, 5}); // Any, GameDirectors, Admin, Host, Owner, Internal
    List actions{0};
    // Mine, DoorsAndSwitches, OpenContainers, AttackPlayers, AttackMobs, Operator, Teleport, Build, Default
    for (int b : {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100}) actions.push_back(b);
    actions.push_back(0x1FF);
    p.values("mActionPermissions", actions);
    p.values("mPlayerPermissionLevel", {0, 1, 2, 3}); // Visitor, Member, Operator, Custom
    p.values("mUniqueId", {-1, 1, std::int64_t{-4294967295LL}});
    p.values("mCustomStoredPermissions", {0, 0x1FF});
}
