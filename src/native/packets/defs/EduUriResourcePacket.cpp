#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("EduUriResourcePacket") {
    p.since("1.17.30");
    p.shape("1.17.30", {
        string("edu_shared_uri_resource_link_uri", 0x0),
        string("edu_shared_uri_resource_button_name", 0x20),
    });

    p.values("edu_shared_uri_resource_link_uri", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("edu_shared_uri_resource_button_name", {"", "hello", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
}
