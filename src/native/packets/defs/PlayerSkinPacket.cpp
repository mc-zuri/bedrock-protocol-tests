#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Layout skin(bool playFab, std::size_t newName, std::size_t oldName) {
    Layout l{uuid("mUUID", 0x0), string("mSkinId", 0x10)};
    if (playFab) l.push_back(string("mPlayFabId", 0x30));
    l.push_back(string("mLocalizedNewSkinName", newName));
    l.push_back(string("mLocalizedOldSkinName", oldName));
    return l;
}
}

BPX_PACKET("PlayerSkinPacket") {
    p.shape("1.16.201", skin(false, 0x1E0, 0x200));
    p.shape("1.16.210", skin(true, 0x200, 0x220));
    p.shape("1.17.0", skin(true, 0x210, 0x230));
    p.shape("1.17.30", skin(true, 0x278, 0x298));
    p.shape("1.19.50", skin(true, 0x260, 0x280));
    p.shape("1.20.50", skin(true, 0x270, 0x290));
    p.shape("1.21.30", skin(true, 0x278, 0x298));
    p.shape("1.21.60", skin(true, 0x220, 0x240));
    p.shape("1.21.90", {uuid("mUUID", 0x0), string("mLocalizedNewSkinName", 0x20), string("mLocalizedOldSkinName", 0x40)});

    p.values("mUUID", {Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{0x0FEDCBA987654321LL}}}});
    p.values("mSkinId", {"", "c18e65aa-7b21-4637-9b63-8ad63622ef01.Custom"});
    p.values("mPlayFabId", {"", "a1b2c3d4e5f60718"});
    p.values("mLocalizedNewSkinName", {"", "Steve", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    p.values("mLocalizedOldSkinName", {"", "Alex"});
}
