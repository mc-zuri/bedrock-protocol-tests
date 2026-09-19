#pragma once

namespace bpx::bindings::names {

inline constexpr char const* CreatePacket     = "MinecraftPackets::createPacket";
inline constexpr char const* BinaryStreamCtor = "BinaryStream::BinaryStream(string&,bool)";

inline constexpr char const* BinaryStreamVtable = "BinaryStream::vftable";

inline constexpr char const* PacketGetName = "Packet::getName";
inline constexpr char const* PacketWrite   = "Packet::write";
inline constexpr char const* PacketRead    = "Packet::read";

inline constexpr char const* PacketSize            = "Packet::size";                       // = payload offset
inline constexpr char const* ResultHasValue        = "Bedrock::Result<void>::has_value";
inline constexpr char const* MaxPacketId           = "MinecraftPacketIds::max";

inline constexpr char const* StreamOwnedBuffer   = "BinaryStream::mOwnedBuffer";
inline constexpr char const* StreamView          = "BinaryStream::mView";
inline constexpr char const* StreamReadPointer   = "BinaryStream::mReadPointer";
inline constexpr char const* StreamHasOverflowed = "BinaryStream::mHasOverflowed";
inline constexpr char const* StreamBuffer        = "BinaryStream::mBuffer";

inline constexpr char const* GetNameKind        = "Packet::getName kind";
inline constexpr char const* StreamConstruction = "BinaryStream construction";
inline constexpr char const* ReadKind           = "Packet::read kind";

enum StreamConstructionValue : int {
    StreamByConstructor = 0,
    StreamByLayout      = 1, // no out-of-line constructor (1.26.30+): vtable + members set here
};

enum ReadKindValue : int {
    ReadResult   = 0, // Bedrock::Result<void, std::error_code> via hidden pointer; ok = has_value byte (1.20.10+)
    ReadEnum     = 1, // StreamReadResult in eax; 1 = Valid (1.16.201 - 1.19.80)
    ReadExtended = 2, // ExtendedStreamReadResult via hidden pointer: StreamReadResult, then a std::string (1.20.0)
};

enum GetNameKindValue : int {
    GetNameString     = 0,
    GetNameStringView = 1,
};

}
