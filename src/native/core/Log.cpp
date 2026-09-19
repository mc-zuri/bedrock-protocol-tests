#include "core/Log.h"

#include <cstdio>
#include <mutex>

namespace bpx::log {

namespace {
std::FILE* gFile = nullptr;
std::mutex gMutex;
}

void open(std::string const& path) {
    std::lock_guard lock(gMutex);
    if (gFile) std::fclose(gFile);
    gFile = std::fopen(path.c_str(), "wb");
}

void line(std::string_view text) {
    std::lock_guard lock(gMutex);
    if (!gFile) return;
    std::fwrite(text.data(), 1, text.size(), gFile);
    std::fputc('\n', gFile);
    std::fflush(gFile);
}

}
