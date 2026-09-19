#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;

Layout base() { return {string("mName", 0x0), blockPos("mPos", 0x20), f32("mVolume", 0x2C), f32("mPitch", 0x30)}; }
Field handle() { return optional("mServerSoundHandle", 0x38, 8, u64("value", 0)); }
}

BPX_PACKET("PlaySoundPacket") {
    p.shape("1.16.201", base());
    {
        Layout l = base();
        l.push_back(handle());
        p.shape("1.26.20", l);
    }
    {
        Layout l = base();
        l.push_back(i32("mLoopCount", 0x34));
        l.push_back(handle());
        p.shape("1.26.40", l);
        l.push_back(boolean("mBypassListenerRangeCheck", 0x48));
        l.push_back(optional("mPlaybackPositionSeconds", 0x4C, 4, f32("value", 0)));
        p.shape("1.26.50", l);
    }

    p.baseline("mName", "random.click");
    p.values("mName", {"random.click", "note.harp", "custom.pack.sound", "\xc2\xa7" "aunicode \xe2\x9c\x93"});
    // positions are sent in eighths of a block: y within the world
    p.values("mPos", {Object{{"x", 0}, {"y", 512}, {"z", 0}}, Object{{"x", -988}, {"y", -512}, {"z", 3648}},
                      Object{{"x", 240000}, {"y", 2552}, {"z", -240000}}});
    p.values("mVolume", {0.0, 0.5, 1.0, 4.0});
    p.values("mPitch", {0.5, 1.0, 2.0});
    p.values("mLoopCount", {0, 1, 5, -1});
    p.values("mServerSoundHandle", {Value(), 0, 1, std::int64_t{123456789012LL}});
    p.values("mBypassListenerRangeCheck", {false, true});
    p.values("mPlaybackPositionSeconds", {Value(), 0.0, 1.5, 120.25});
}
