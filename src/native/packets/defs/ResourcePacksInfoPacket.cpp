#include "packets/Packet.h"

#include <cstdint>

BPX_PACKET("ResourcePacksInfoPacket") {
    p.shape("1.16.201", {boolean("mResourcePackRequired", 0x0), boolean("mHasScripts", 0x1)});
    p.shape("1.17.10", {boolean("mResourcePackRequired", 0x0), boolean("mHasScripts", 0x1), boolean("mForceServerPacks", 0x3)});
    p.shape("1.20.71", {boolean("mResourcePackRequired", 0x0), boolean("mHasAddonPacks", 0x1), boolean("mHasScripts", 0x2),
                        boolean("mForceServerPacks", 0x4)});
    p.shape("1.21.30", {boolean("mResourcePackRequired", 0x0), boolean("mHasAddonPacks", 0x1), boolean("mHasScripts", 0x2)});
    p.shape("1.21.50", {boolean("mResourcePackRequired", 0x0), boolean("mHasAddonPacks", 0x1), boolean("mHasScripts", 0x2),
                        uuid("mWorldTemplateId", 0x8)});
    p.shape("1.21.90", {boolean("mResourcePackRequired", 0x0), boolean("mHasAddonPacks", 0x1), boolean("mHasScripts", 0x2),
                        boolean("mForceDisableVibrantVisuals", 0x3), uuid("mWorldTemplateId", 0x8)});

    for (char const* f : {"mResourcePackRequired", "mHasScripts", "mForceServerPacks", "mHasAddonPacks", "mForceDisableVibrantVisuals"})
        p.values(f, {false, true});
    p.values("mWorldTemplateId", {Object{{"high", 0}, {"low", 0}},
                                  Object{{"high", std::int64_t{0x0123456789ABCDEFLL}}, {"low", std::int64_t{-0x0FEDCBA987654322LL}}}});
}
