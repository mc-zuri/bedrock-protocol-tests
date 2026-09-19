// The reader rejects the empty default compound, so the packet is given as network NBT bytes.
#include "packets/Nbt.h"
#include "packets/Packet.h"

BPX_PACKET("SyncActorPropertyPacket") {
    namespace n = bpx::packets::nbt;
    p.payload("one int", n::root(n::intTag("x", 1)));
    p.payload("actor properties",
              n::root(n::stringTag("type", "minecraft:bee") + n::compoundList("properties", {n::intTag("index", 0) + n::intTag("value", 1),
                                                                                             n::intTag("index", 1) + n::floatTag("value", 0.5f)})));
}
