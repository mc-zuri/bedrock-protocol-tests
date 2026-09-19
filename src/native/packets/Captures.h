// Reads <BPX_CAPTURES>/<version>/captured-packets.bin (export/Dump.h format, written by `pnpm capture`):
// each 'C' record is a wire packet, a varuint header (id in its low 10 bits) then the payload.
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace bpx::packets::captures {

struct Capture {
    std::string   name; // bedrock-protocol's packet name
    std::uint32_t id = 0;
    std::string   payload;
};

namespace detail {

inline bool varint(std::string const& s, std::size_t& at, std::uint64_t& value) {
    value = 0;
    for (int shift = 0; at < s.size() && shift < 64; shift += 7) {
        auto b = static_cast<unsigned char>(s[at++]);
        value |= static_cast<std::uint64_t>(b & 0x7F) << shift;
        if (!(b & 0x80)) return true;
    }
    return false;
}

// no JSON unescaping: names are plain identifiers
inline std::string name(std::string_view meta) {
    auto at = meta.find(R"("p":")");
    if (at == std::string_view::npos) return {};
    at += 5;
    return std::string(meta.substr(at, meta.find('"', at) - at));
}

}

inline std::filesystem::path root() {
    char const* r = std::getenv("BPX_CAPTURES");
    return r ? std::filesystem::path(r) : std::filesystem::path();
}

inline std::vector<Capture> all(std::string_view version) {
    std::vector<Capture> out;
    if (root().empty()) return out;
    std::ifstream in(root() / std::string(version) / "captured-packets.bin", std::ios::binary);
    if (!in) return out;
    std::string const s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    std::size_t       at = 0;
    std::uint64_t     n  = 0;
    if (!detail::varint(s, at, n)) return {};
    at += n; // the version
    std::string meta;
    while (at < s.size()) {
        char const tag = s[at];
        at += 1 + 8; // tag + time
        if (tag == 'L') {
            if (!detail::varint(s, at, n) || at + n > s.size()) return {};
            meta = s.substr(at, n);
            at += n;
        } else if (tag == 'C') {
            if (at + 4 > s.size()) return {};
            std::int32_t len = 0;
            std::copy_n(s.data() + at, 4, reinterpret_cast<char*>(&len));
            at += 4;
            if (len < 0 || at + len > s.size()) return {};
            std::string const packet = s.substr(at, len);
            at += len;
            std::size_t   body   = 0;
            std::uint64_t header = 0;
            if (detail::varint(packet, body, header))
                out.push_back({detail::name(meta), static_cast<std::uint32_t>(header & 0x3FF), packet.substr(body)});
            meta.clear();
        } else {
            return {};
        }
    }
    std::ranges::sort(out, {}, &Capture::name);
    return out;
}

inline std::optional<std::string> payload(std::string_view version, std::string_view packet) {
    for (auto& c : all(version))
        if (c.name == packet) return std::move(c.payload);
    return std::nullopt;
}

}
