#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("EditorNetworkPacket") {
    p.since("1.19.10");
    p.shape("1.19.10", {});
    p.shape("1.21.20", {
        boolean("route_to_manager", 0x18),
    });
    p.shape("1.21.120", {});
    p.shape("1.21.124", {
        boolean("route_to_manager", 0x18),
    });
    p.shape("1.26.10", {
        string("raw_variant_name", 0x0),
        string("raw_variant_data", 0x20),
        boolean("route_to_manager", 0x40),
    });

    p.values("route_to_manager", {false, true});
    p.values("raw_variant_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("raw_variant_data", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
