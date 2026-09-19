#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace bpx::memory {

struct Module {
    std::uintptr_t base = 0;
    std::uintptr_t textBegin = 0, textEnd = 0;

    static Module const& game();

    [[nodiscard]] std::uintptr_t rva(std::uintptr_t address) const { return address - base; }
};

// IDA syntax: "48 8B 05 ? ? ? ? 48 33 C4"
class Pattern {
public:
    static std::optional<Pattern> parse(std::string_view text);

    [[nodiscard]] std::vector<std::uintptr_t> find(std::uintptr_t begin, std::uintptr_t end, std::size_t limit = 2) const;

    [[nodiscard]] std::size_t size() const { return mBytes.size(); }

private:
    std::vector<std::uint8_t> mBytes;
    std::vector<bool>         mWild;
};

// `at` points at a rel32; `trailing` = instruction bytes after it (e.g. an imm8).
inline std::uintptr_t relTarget(std::uintptr_t at, int trailing = 0) {
    return at + 4 + trailing + static_cast<std::intptr_t>(*reinterpret_cast<std::int32_t const*>(at));
}

}
