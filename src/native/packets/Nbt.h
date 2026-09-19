// Network NBT as VarIntDataOutput writes it: ints are zigzag varints, lengths are varuints.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace bpx::packets::nbt {

inline std::string varuint(std::uint64_t v) {
    std::string out;
    do {
        std::uint8_t b = v & 0x7F;
        v >>= 7;
        out += static_cast<char>(v ? b | 0x80 : b);
    } while (v);
    return out;
}
inline std::string zigzag(std::int64_t v) { return varuint((static_cast<std::uint64_t>(v) << 1) ^ static_cast<std::uint64_t>(v >> 63)); }
inline std::string name(std::string_view n) { return varuint(n.size()) + std::string(n); }

inline std::string byteTag(std::string_view n, std::int8_t v) { return "\x01" + name(n) + std::string(1, static_cast<char>(v)); }
inline std::string intTag(std::string_view n, std::int32_t v) { return "\x03" + name(n) + zigzag(v); }
inline std::string floatTag(std::string_view n, float v) {
    std::string out = "\x05" + name(n);
    out.append(reinterpret_cast<char const*>(&v), 4);
    return out;
}
inline std::string stringTag(std::string_view n, std::string_view v) { return "\x08" + name(n) + varuint(v.size()) + std::string(v); }
inline std::string compoundList(std::string_view n, std::initializer_list<std::string> items) {
    std::string out = "\x09" + name(n) + "\x0a" + zigzag(static_cast<std::int64_t>(items.size()));
    for (auto const& i : items) out += i + std::string(1, '\0');
    return out;
}
inline std::string compoundTag(std::string_view n, std::string const& entries) { return "\x0a" + name(n) + entries + std::string(1, '\0'); }
inline std::string root(std::string const& entries) { return compoundTag("", entries); }

}
