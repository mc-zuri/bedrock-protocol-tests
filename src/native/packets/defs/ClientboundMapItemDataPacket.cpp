#include "packets/MapItemData.h"

BPX_PACKET("ClientboundMapItemDataPacket") {
    namespace m = bpx::packets::mapitem;
    p.since("1.21.60");
    p.shape("1.21.60", m::layoutB());
    p.shape("1.26.40", m::layoutC());
    m::values(p);
}
