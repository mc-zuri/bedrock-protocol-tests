#include "packets/Packet.h"

BPX_PACKET("ServerStoreInfoPacket") {
    p.since("1.26.20");
    p.shape("1.26.20", {
        optional("mClientStoreEntryPointConfiguration", 0x0, 0x40,
                 structure("value", 0, {string("storeId", 0x0), string("storeName", 0x20)})),
    });

    p.values("mClientStoreEntryPointConfiguration", {Value(), Object{{"storeId", ""}, {"storeName", ""}},
                                                     Object{{"storeId", "store-1234"}, {"storeName", "Server Shop"}}});
}
