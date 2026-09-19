#include "packets/Packet.h"

#include <cstdint>
#include <string>

BPX_PACKET("ClientboundDataDrivenUIShowScreenPacket") {
    p.since("1.26.0");
    p.shape("1.26.0", {string("mScreenId", 0x0)});
    p.shape("1.26.10", {string("mScreenId", 0x0), u32("mFormId", 0x20), optional("mDataInstanceId", 0x24, 4, u32("value", 0))});

    p.values("mScreenId", {"", "my_pack:main_menu", std::string(500, 's')}); // 500: the max length
    p.values("mFormId", {0, 1, std::int64_t{4294967295LL}});
    p.values("mDataInstanceId", {Value(), 0, 7, std::int64_t{4294967295LL}});
}
