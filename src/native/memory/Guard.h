#pragma once

#include <cstdint>

namespace bpx::memory {

// Returns false if `fn` raised a structured exception, so one bad game call does not kill the server.
using GuardedFn = void (*)(void* ctx);
bool guarded(GuardedFn fn, void* ctx, unsigned long* code = nullptr);

// rva is an absolute address when !inImage.
struct Fault {
    unsigned long  code    = 0;
    std::uintptr_t rva     = 0;
    bool           inImage = false;
};
Fault lastFault();

template <class F>
bool guard(F&& f, unsigned long* code = nullptr) {
    return guarded([](void* p) { (*static_cast<F*>(p))(); }, &f, code);
}

}
