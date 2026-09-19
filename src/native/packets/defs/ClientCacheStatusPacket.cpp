#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ClientCacheStatusPacket") {
    p.shape("1.16.201", {});
    p.shape("1.16.210", {
        boolean("is_cache_supported", 0x0),
    });

    p.values("is_cache_supported", {false, true});
}
