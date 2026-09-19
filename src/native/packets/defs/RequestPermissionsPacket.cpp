#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("RequestPermissionsPacket") {
    p.since("1.19.1");
    p.shape("1.19.1", {
        i64("target_player_ids_raw_id", 0x0),
        u8("player_permission_level", 0x8),
        i16("custom_permission_flags", 0xA),
    });

    p.values("target_player_ids_raw_id", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("player_permission_level", {0, 1, 2, 3}); // PlayerPermissionLevel: Visitor, Member, Operator, Custom
    p.values("custom_permission_flags", {0, 1, -1, 32767, -32768});
}
