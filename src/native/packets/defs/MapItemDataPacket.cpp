#include "packets/MapItemData.h"

BPX_PACKET("MapItemDataPacket") {
    namespace m = bpx::packets::mapitem;
    p.until("1.21.60");
    p.shape("1.16.201", m::layoutA());
    p.shape("1.19.20", m::layoutB());
    m::values(p);
}
