#include "packets/Packet.h"

BPX_PACKET("DataStoreSyncPacket") {
    p.since("1.21.120");
    p.until("1.21.130");
    p.shape("1.21.120", {vector("mUpdate", 0x0, 0x98,
                                variant("value", 0, 0x90,
                                        {structure("DataStoreChange", 0, {string("mDataStoreName", 0x0), string("mProperty", 0x20),
                                                                          u32("mUpdateCount", 0x40)}),
                                         structure("DataStoreRemoval", 0, {string("mDataStoreName", 0x0)})}))});

    // a built entry is written as its variant index alone, which the reader rejects: only the empty list is sent
    p.values("mUpdate", {List{}});
}
