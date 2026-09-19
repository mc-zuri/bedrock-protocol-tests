#include "packets/Packet.h"

BPX_PACKET("ItemStackRequestPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {pointer("mRequestBatch", 0x0, 0x18, structure("value", 0, {vector("mRequests", 0x0, 0x8, u64("value", 0))}))});
    p.shape("1.26.40", {vector("mRequests", 0x0, 0x8, u64("value", 0))});

    // a null batch is written as nothing, which the reader rejects
    p.baseline("mRequestBatch", Object{{"mRequests", List{}}});
}
