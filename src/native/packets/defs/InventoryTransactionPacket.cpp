#include "packets/Packet.h"

#include <climits>
#include <string>

BPX_PACKET("InventoryTransactionPacket") {
    p.shape("1.26.30", {i32("mLegacyRequestId", 0x8)});

    using namespace std::string_literals;
    // until 1.26.30 the transaction is a polymorphic object (null by default), so the reader builds it from bytes:
    // zigzag legacy request id, varuint type (Normal 0, InventoryMismatch 1), [1.16.201: bool network ids], varuint action count
    p.payload("1.16.210", "1.26.30", "normal", "\x00\x00\x00"s);
    p.payload("1.16.210", "1.26.30", "inventory mismatch", "\x00\x01\x00"s);
    // legacy request id -2, then 1 container: 12 (hotbar) with slots 0 and 1, then a normal transaction
    p.payload("1.16.210", "1.26.30", "legacy request", "\x03\x01\x0c\x02\x00\x01\x00\x00"s);
    for (bool network : {false, true}) {
        std::string const n(1, network ? '\x01' : '\x00'), tag = network ? " with network ids" : "";
        p.payload("1.16.201", "1.16.210", "normal" + tag, "\x00\x00"s + n + "\x00"s);
        p.payload("1.16.201", "1.16.210", "inventory mismatch" + tag, "\x00\x01"s + n + "\x00"s);
        p.payload("1.16.201", "1.16.210", "legacy request" + tag, "\x03\x01\x0c\x02\x00\x01\x00"s + n + "\x00"s);
    }

    p.values("mLegacyRequestId", {0, 1, -1, 1000, INT_MAX, INT_MIN});
}
