#include "server/Recorder.h"

#include "memory/Scanner.h"

#include <windows.h>

#include <cstring>

namespace {

constexpr int kSlots = 64;

struct Active {
    void const*                           stream = nullptr;
    std::string const*                    buffer = nullptr;
    std::vector<bpx::server::WriteEvent>* events = nullptr;
    void*                                 original = nullptr;
};

thread_local Active gActive;

// Not every slot takes a doc name in r8, so only accept a short printable string inside the game image.
bool docName(char const* p, std::string& out) {
    static auto const range = [] {
        auto const& g = bpx::memory::Module::game();
        auto        dos = reinterpret_cast<IMAGE_DOS_HEADER const*>(g.base);
        auto        nt  = reinterpret_cast<IMAGE_NT_HEADERS64 const*>(g.base + dos->e_lfanew);
        return std::pair{g.base, g.base + nt->OptionalHeader.SizeOfImage};
    }();
    auto a = reinterpret_cast<std::uintptr_t>(p);
    if (a < range.first || a >= range.second - 96) return false;
    std::size_t n = 0;
    while (n < 80 && p[n]) {
        if (static_cast<unsigned char>(p[n]) < 32 || static_cast<unsigned char>(p[n]) > 126) return false;
        ++n;
    }
    if (n == 0 || p[n]) return false;
    out.assign(p, n);
    return true;
}

void* gTable[kSlots]{};
void* gFor = nullptr;

}

extern "C" {
void* bpx_original_slots[kSlots]{};
extern void* bpx_thunk_table[kSlots];

// r9 passes through the thunk untouched: the writer's second doc string.
void bpx_stream_call(void* stream, unsigned long long slot, char const* doc, char const* notes) {
    auto& a = gActive;
    if (!a.events || stream != a.stream) return;
    std::string name, extra;
    if (!docName(doc, name)) return;
    docName(notes, extra);
    a.events->push_back({std::move(name), std::move(extra), a.buffer->size(), static_cast<int>(slot)});
}
}

namespace bpx::server::recorder {

bool attach(void* stream, std::string const* buffer, std::vector<WriteEvent>* events) {
    auto** vt = *static_cast<void***>(stream);
    auto const& g = memory::Module::game();
    if (gFor != vt) {
        // Slots 0 (destructor) and 1 (read) stay direct. Entries past the real table's end are
        // copied too but never called.
        for (int k = 0; k < kSlots; ++k) {
            auto p               = reinterpret_cast<std::uintptr_t>(vt[k]);
            bool code            = p >= g.textBegin && p < g.textEnd;
            bpx_original_slots[k] = vt[k];
            gTable[k]             = (k >= 2 && code) ? bpx_thunk_table[k] : vt[k];
        }
        gFor = vt;
    }
    gActive = {stream, buffer, events, vt};
    *static_cast<void**>(stream) = gTable;
    return true;
}

void detach(void* stream) {
    if (gActive.stream == stream && gActive.original) *static_cast<void**>(stream) = gActive.original;
    gActive = {};
}

}
