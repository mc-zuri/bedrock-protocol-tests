#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ShowStoreOfferPacket") {
    p.shape("1.16.201", {string("mOfferId", 0x0), boolean("mShowAll", 0x40)});
    p.shape("1.20.50", {uuid("mOfferUuid", 0x0), u8("mRedirectType", 0x10)});

    Object const id{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}};
    p.baseline("mOfferUuid", id); // sent as text; the all-zero UUID is not read back
    p.values("mOfferId", {"", "8a3b8fa9-0e6b-4d6a-9f2e-3c1d2b4a5f60"});
    p.values("mShowAll", {false, true});
    p.values("mOfferUuid", {id, Object{{"high", std::int64_t{1}}, {"low", std::int64_t{2}}}});
    p.values("mRedirectType", {0, 1, 2}); // MarketplaceOffer, DressingRoomOffer, ThirdPartyServerPage
}
