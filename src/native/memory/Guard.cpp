#include "memory/Guard.h"

#include <windows.h>

namespace bpx::memory {

namespace {

thread_local Fault tLast;

int record(EXCEPTION_POINTERS* info) {
    tLast.code      = info->ExceptionRecord->ExceptionCode;
    auto const at   = reinterpret_cast<std::uintptr_t>(info->ExceptionRecord->ExceptionAddress);
    auto const base = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
    auto const* dos = reinterpret_cast<IMAGE_DOS_HEADER const*>(base);
    auto const* nt  = reinterpret_cast<IMAGE_NT_HEADERS const*>(base + dos->e_lfanew);
    tLast.inImage   = at >= base && at < base + nt->OptionalHeader.SizeOfImage;
    tLast.rva       = tLast.inImage ? at - base : at;
    return EXCEPTION_EXECUTE_HANDLER;
}

}

Fault lastFault() { return tLast; }

// Nothing with a destructor may live in this frame: __try cannot unwind C++ objects.
bool guarded(GuardedFn fn, void* ctx, unsigned long* code) {
    __try {
        fn(ctx);
        return true;
    } __except (record(GetExceptionInformation())) {
        if (code) *code = tLast.code;
        return false;
    }
}

}
