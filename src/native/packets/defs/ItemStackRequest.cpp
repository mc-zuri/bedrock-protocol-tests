#include "packets/Packet.h"

#include <string>

BPX_PACKET("ItemStackRequest") {
    p.until("1.20.50");
    using namespace std::string_literals;
    // the default batch is null and written as nothing, which the reader rejects, so the reader builds it from bytes:
    // varuint request count; each a zigzag request id (a client's: negative), varuint action count, varuint filter
    // string count, and from 1.19.30 an i32 LE filter cause
    p.payload("no requests", "\x00"s);
    p.payload("0.0.0", "1.19.30", "one request", "\x01\x01\x00\x00"s);
    p.payload("0.0.0", "1.19.30", "two requests", "\x02\x01\x00\x00\x05\x00\x00"s);
    p.payload("1.19.30", "9999.0.0", "one request", "\x01\x01\x00\x00\x00\x00\x00\x00"s);
    p.payload("1.19.30", "9999.0.0", "two requests", "\x02\x01\x00\x00\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00"s);
}
