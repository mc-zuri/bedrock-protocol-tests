#pragma once

#include <cstdint>
#include <string>
#include <string_view>

// BDT: varint(len) version, then records of tag byte + int64 time (0):
//   'L' varint(len) JSON meta for the next 'C';  'C' int32le(len) varint(packet id) + body
namespace bpx::dump {

void putVarint(std::string& out, std::uint64_t value);

class Writer {
public:
    explicit Writer(std::string_view version);

    void packet(std::uint32_t id, std::string_view meta, std::string_view body);

    [[nodiscard]] std::string const& bytes() const { return mOut; }
    [[nodiscard]] std::size_t        count() const { return mCount; }

private:
    std::string mOut;
    std::size_t mCount = 0;
};

std::string quote(std::string_view s);

}
