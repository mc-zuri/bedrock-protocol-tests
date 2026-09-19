#include "packets/Packet.h"

namespace {
using namespace bpx::schema;
Value alt(int index) { return Object{{"index", index}, {"value", Object{}}}; }
}

BPX_PACKET("ResourcePackClientResponsePacket") {
    p.shape("1.16.201", {u8("mResponse", 0x10)});
    p.shape("1.26.40", {variant("mResponseVariant", 0x0, 0x10, {structure("Cancel", 0, {}), structure("Downloading", 0, {}),
                                                                 structure("DownloadingFinished", 0, {}),
                                                                 structure("ResourcePackStackFinished", 0, {})})});

    p.baseline("mResponse", 1);
    p.values("mResponse", {1, 2, 3, 4}); // Cancel, Downloading, DownloadingFinished, ResourcePackStackFinished
    p.values("mResponseVariant", {alt(0), alt(2), alt(3)}); // not Downloading: its pack set is not built
}
