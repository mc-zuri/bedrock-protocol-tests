#include "packets/Packet.h"

BPX_PACKET("BiomeDefinitionList") {
    p.until("1.21.60");
    // BiomeDefinitionListPacket's old name; the reader rejects the default empty CompoundTag, so the body is captured
    p.captured("biome_definition_list");
}
