#include "export/Dump.h"

#include <cstdio>

namespace bpx::dump {

void putVarint(std::string& out, std::uint64_t value) {
    do {
        auto b = static_cast<std::uint8_t>(value & 0x7F);
        value >>= 7;
        if (value) b |= 0x80;
        out.push_back(static_cast<char>(b));
    } while (value);
}

namespace {
void putTime(std::string& out) { out.append(8, '\0'); }
}

Writer::Writer(std::string_view version) {
    putVarint(mOut, version.size());
    mOut.append(version);
}

void Writer::packet(std::uint32_t id, std::string_view meta, std::string_view body) {
    if (!meta.empty()) {
        mOut.push_back('L');
        putTime(mOut);
        putVarint(mOut, meta.size());
        mOut.append(meta);
    }
    std::string frame;
    putVarint(frame, id);
    frame.append(body);
    mOut.push_back('C');
    putTime(mOut);
    auto len = static_cast<std::int32_t>(frame.size());
    mOut.append(reinterpret_cast<char const*>(&len), 4);
    mOut.append(frame);
    ++mCount;
}

std::string quote(std::string_view s) {
    std::string out = "\"";
    for (char c : s) {
        switch (c) {
        case '"': out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default:
            if (static_cast<unsigned char>(c) < 0x20) {
                char buf[8];
                std::snprintf(buf, sizeof buf, "\\u%04x", c);
                out += buf;
            } else {
                out += c;
            }
        }
    }
    return out + "\"";
}

}
