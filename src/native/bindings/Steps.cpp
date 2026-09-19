#include "bindings/Bindings.h"

BPX_BINDINGS("1.16.201") {
    r.function("MinecraftPackets::createPacket", "48 8B C4 55 41 56 41 57 48 8B EC 48 81 EC 80 00 00 00 48 C7 45 B0 FE FF FF FF 48 89 58 10");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 8B C4 57 48 83 EC 60 48 C7 40 E0 FE FF FF FF 48 89 58 18 48 8B DA 48 8B F9");
    r.slot("Packet::getName", 2);
    r.slot("Packet::write", 3);
    r.slot("Packet::read", 4);
    r.offset("Packet::size", 40);
    r.offset("MinecraftPacketIds::max", 163);
    r.variant("Packet::getName kind", 0);
    r.variant("BinaryStream construction", 0);
    r.variant("Packet::read kind", 1);
}

BPX_BINDINGS("1.16.210") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 30 48 8B F9");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 18 55 56 57 41 56 41 57 48 83 EC 70 41 0F B6 E8");
    r.slot("Packet::read", 6);
    r.offset("Packet::size", 48);
    r.offset("MinecraftPacketIds::max", 164);
}

BPX_BINDINGS("1.17.0") {
    r.offset("MinecraftPacketIds::max", 167);
}

BPX_BINDINGS("1.17.10") {
    r.offset("MinecraftPacketIds::max", 169);
}

BPX_BINDINGS("1.17.30") {
    r.offset("MinecraftPacketIds::max", 172);
}

BPX_BINDINGS("1.17.40") {
    r.offset("MinecraftPacketIds::max", 175);
}

BPX_BINDINGS("1.18.11") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 40 48 8B 05 ? ? ? ?");
    r.offset("MinecraftPacketIds::max", 178);
}

BPX_BINDINGS("1.18.30") {
    r.offset("MinecraftPacketIds::max", 182);
}

BPX_BINDINGS("1.19.1") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 30 48 8B D9");
    r.offset("MinecraftPacketIds::max", 186);
}

BPX_BINDINGS("1.19.10") {
    r.offset("MinecraftPacketIds::max", 190);
}

BPX_BINDINGS("1.19.20") {
    r.offset("MinecraftPacketIds::max", 191);
}

BPX_BINDINGS("1.19.30") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 40");
    r.offset("MinecraftPacketIds::max", 195);
}

BPX_BINDINGS("1.19.50") {
    r.offset("MinecraftPacketIds::max", 196);
}

BPX_BINDINGS("1.19.60") {
    r.offset("MinecraftPacketIds::max", 197);
}

BPX_BINDINGS("1.19.70") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 40 48 8B 05 ? ? ? ?");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 18 55 56 57 41 56 41 57 48 83 EC 70 41 0F B6 E8 48 8B FA");
    r.offset("MinecraftPacketIds::max", 300);
}

BPX_BINDINGS("1.19.80") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0 48 8B D9");
    r.slot("Packet::read", 7);
    r.offset("MinecraftPacketIds::max", 303);
}

BPX_BINDINGS("1.20.0") {
    r.variant("Packet::read kind", 2);
}

BPX_BINDINGS("1.20.10") {
    r.offset("MinecraftPacketIds::max", 304);
    r.offset("Bedrock::Result<void>::has_value", 64);
    r.variant("Packet::read kind", 0);
}

BPX_BINDINGS("1.20.30") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8B EC 48 83 EC 40 48 8B F9 48 89 4D E8");
    r.offset("MinecraftPacketIds::max", 305);
}

BPX_BINDINGS("1.20.50") {
    r.offset("MinecraftPacketIds::max", 307);
}

BPX_BINDINGS("1.20.61") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8B EC 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0");
    r.offset("MinecraftPacketIds::max", 308);
}

BPX_BINDINGS("1.20.71") {
    r.slot("Packet::write", 4);
    r.slot("Packet::read", 8);
}

BPX_BINDINGS("1.21.0") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8B EC 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0 48 8B F9");
    r.slot("Packet::read", 5);
    r.offset("MinecraftPacketIds::max", 309);
}

BPX_BINDINGS("1.21.2") {
    r.offset("MinecraftPacketIds::max", 310);
}

BPX_BINDINGS("1.21.20") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 B9 48 81 EC A0 00 00 00 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 3F 48 8B F9");
    r.offset("MinecraftPacketIds::max", 315);
}

BPX_BINDINGS("1.21.30") {
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 18 48 89 6C 24 20 56 57 41 56 48 83 EC 70 41 0F B6 E8");
    r.offset("MinecraftPacketIds::max", 317);
}

BPX_BINDINGS("1.21.42") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 40 48 8B D9");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC 70 45 0F B6 E0");
}

BPX_BINDINGS("1.21.50") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F0 48 8B D9");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 40 48 83 7A 18 0F");
    r.offset("MinecraftPacketIds::max", 320);
}

BPX_BINDINGS("1.21.60") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 50 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 48 48 8B D9 48 89 4C 24 38");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 10 57 48 83 EC 30 48 89 4C 24 20 48 8B FA");
    r.offset("MinecraftPacketIds::max", 322);
}

BPX_BINDINGS("1.21.70") {
    r.offset("MinecraftPacketIds::max", 325);
}

BPX_BINDINGS("1.21.80") {
    r.offset("MinecraftPacketIds::max", 327);
}

BPX_BINDINGS("1.21.90") {
    r.offset("MinecraftPacketIds::max", 328);
}

BPX_BINDINGS("1.21.100") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 60 48 8B F9");
    r.slot("Packet::write", 6);
    r.slot("Packet::read", 8);
}

BPX_BINDINGS("1.21.111") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 60 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 58 48 8B F9");
    r.slot("Packet::write", 5);
    r.slot("Packet::read", 7);
    r.offset("MinecraftPacketIds::max", 329);
}

BPX_BINDINGS("1.21.120") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 60 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 58 48 8B F9 48 89 4C 24 38");
    r.slot("Packet::write", 6);
    r.slot("Packet::read", 8);
    r.offset("MinecraftPacketIds::max", 331);
}

BPX_BINDINGS("1.21.130") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 68 48 8B F9 48 89 4C 24 58");
    r.function("BinaryStream::BinaryStream(string&,bool)", "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 30 48 89 4C 24 20");
    r.offset("MinecraftPacketIds::max", 332);
}

BPX_BINDINGS("1.26.0") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8B EC 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F8 48 8B F9");
    r.offset("MinecraftPacketIds::max", 339);
    r.variant("Packet::getName kind", 1);
}

BPX_BINDINGS("1.26.10") {
    r.function("MinecraftPackets::createPacket", "48 89 5C 24 10 48 89 74 24 18 48 89 7C 24 20 55 48 8B EC 48 83 EC 70 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 F8");
    r.offset("MinecraftPacketIds::max", 345);
}

BPX_BINDINGS("1.26.20") {
    r.function("MinecraftPackets::createPacket", "56 48 83 EC 20 48 89 CE 81 FA 5C 01 00 00 77 12");
    r.function("BinaryStream::BinaryStream(string&,bool)", "41 57 41 56 41 54 56 57 53 48 83 EC 28 48 89 CE 48 83 7A 18 10");
    r.offset("MinecraftPacketIds::max", 347);
}

BPX_BINDINGS("1.26.30") {
    r.function("MinecraftPackets::createPacket", "56 48 83 EC 20 48 89 CE 81 FA 5F 01 00 00 77 12");
    r.drop("BinaryStream::BinaryStream(string&,bool)");
    r.data("BinaryStream::vftable", "48 8D 0D ? ? ? ? 48 89 8D 80 02 00 00 48 89 85 C8 02 00 00 8B 05 ? ? ? ?");
    r.offset("MinecraftPacketIds::max", 350);
    r.offset("BinaryStream::mOwnedBuffer", 8);
    r.offset("BinaryStream::mView", 40);
    r.offset("BinaryStream::mReadPointer", 56);
    r.offset("BinaryStream::mHasOverflowed", 64);
    r.offset("BinaryStream::mBuffer", 72);
    r.variant("BinaryStream construction", 1);
}

BPX_BINDINGS("1.26.50") {
    r.function("MinecraftPackets::createPacket", "56 48 83 EC 20 48 89 CE 81 FA 61 01 00 00 77 12");
    r.offset("MinecraftPacketIds::max", 352);
}
