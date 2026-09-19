#include "packets/Packet.h"

namespace {
using namespace bpx::schema;

Layout fades(std::size_t at) { return {i32("mFadeInTime", at), i32("mStayTime", at + 4), i32("mFadeOutTime", at + 8)}; }
Layout join(Layout a, Layout const& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}
}

BPX_PACKET("SetTitlePacket") {
    Layout const head{i32("mType", 0x0), string("mTitleText", 0x8)};
    p.shape("1.16.201", join(head, fades(0x28)));
    p.shape("1.17.10", join(join(head, fades(0x28)), {string("mXuid", 0x38), string("mPlatformOnlineId", 0x58)}));
    p.shape("1.21.20", join(join(join(head, {optional("mFilteredTitleText", 0x28, 0x20, string("value", 0))}), fades(0x50)),
                            {string("mXuid", 0x60), string("mPlatformOnlineId", 0x80)}));
    p.shape("1.21.100", join(join(join(head, {string("mFilteredTitleText", 0x28)}), fades(0x48)),
                             {string("mXuid", 0x58), string("mPlatformOnlineId", 0x78)}));

    p.values("mType", {0, 1, 2, 3, 4, 5}); // Clear, Reset, Title, Subtitle, Actionbar, Times
    p.values("1.18.0", "mType", {0, 1, 2, 3, 4, 5, 6, 7, 8}); // + the three TextObject kinds
    p.values("mTitleText", {"", "Welcome!", "\xc2\xa7" "aunicode \xe2\x9c\x93", "{\"rawtext\":[{\"text\":\"hello\"}]}"});
    p.values("mFilteredTitleText", {"", "W******!"});
    p.values("1.21.20", "mFilteredTitleText", {Value(), "", "W******!"});
    p.values("mFadeInTime", {0, 10, 20});
    p.values("mStayTime", {0, 70, 100});
    p.values("mFadeOutTime", {0, 20});
    p.values("mXuid", {"", "2535405123456789"});
    p.values("mPlatformOnlineId", {"", "1234567890"});
}
