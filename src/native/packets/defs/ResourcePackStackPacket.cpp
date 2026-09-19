#include "packets/Packet.h"

#include <climits>
#include <cstdint>

BPX_PACKET("ResourcePackStackPacket") {
    p.since("1.20.50");
    p.shape("1.20.50", {
        boolean("mTexturePackRequired", 0xA8),
        boolean("mExperimentsEverToggled", 0xF0),
    });
    p.shape("1.20.80", {
        boolean("mTexturePackRequired", 0xA8),
        boolean("mExperimentsEverToggled", 0xF0),
        boolean("mIncludeEditorPacks", 0xF8),
    });
    p.shape("1.21.60", {
        boolean("mTexturePackRequired", 0x50),
        boolean("mExperimentsEverToggled", 0x98),
        boolean("mIncludeEditorPacks", 0xA0),
    });
    p.shape("1.21.130", {
        boolean("mTexturePackRequired", 0x38),
        boolean("mExperimentsEverToggled", 0x80),
        boolean("mIncludeEditorPacks", 0x88),
    });

    p.values("mTexturePackRequired", {false, true});
    p.values("mExperimentsEverToggled", {false, true});
    p.values("mIncludeEditorPacks", {false, true});
}
