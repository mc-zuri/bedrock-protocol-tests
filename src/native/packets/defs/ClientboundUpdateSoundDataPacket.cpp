#include "packets/Packet.h"

#include <cstdint>

namespace {
using namespace bpx::schema;
Object alt(int i, Object v) { return Object{{"index", i}, {"value", std::move(v)}}; }
}

BPX_PACKET("ClientboundUpdateSoundDataPacket") {
    p.since("1.26.30");
    p.shape("1.26.30", {u64("mServerSoundHandle", 0x0), i32("mSoundEvent", 0x8)});
    p.shape("1.26.40", {u64("mServerSoundHandle", 0x0),
                        variant("mEvent", 0x8, 0x8,
                                {structure("Stop", 0, {}), structure("SetVolume", 0, {f32("mVolume", 0)}),
                                 structure("SetPitch", 0, {f32("mPitch", 0)}),
                                 structure("Fade", 0, {f32("mDuration", 0), f32("mTargetVolume", 4)}),
                                 structure("SeekTo", 0, {f32("mSeconds", 0)}), structure("Pause", 0, {}), structure("Resume", 0, {})})});

    p.values("mServerSoundHandle", {0, 1, std::int64_t{123456789012LL}});
    p.values("mSoundEvent", {0}); // Stop; the reader rejects the others
    p.values("mEvent", {alt(0, Object{}), alt(1, Object{{"mVolume", 0.5}}), alt(1, Object{{"mVolume", 0.0}}), alt(2, Object{{"mPitch", 1.5}}),
                        alt(3, Object{{"mDuration", 2.0}, {"mTargetVolume", 0.25}}), alt(4, Object{{"mSeconds", 12.75}}), alt(5, Object{}),
                        alt(6, Object{})});
}
