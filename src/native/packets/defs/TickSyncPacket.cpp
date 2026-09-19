#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("TickSyncPacket") {
    p.until("1.21.0");
    p.shape("1.16.201", {
        i64("client_request_timestamp", 0x0),
        i64("server_reception_response_timestamp", 0x8),
    });

    p.values("client_request_timestamp", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
    p.values("server_reception_response_timestamp", {0, 1, -1, (std::int64_t)1000000000000, (std::int64_t)INT64_MAX, (std::int64_t)INT64_MIN});
}
