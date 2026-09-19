#include "core/ThreadContext.h"

#include <windows.h>
#include <tlhelp32.h>

#include <format>

namespace bpx::threads {

namespace {

// offsetof(TEB, ThreadLocalStoragePointer) on x64
constexpr std::size_t kTebTls = 0x58;

struct ThreadBasicInformation {
    LONG      ExitStatus;
    void*     TebBaseAddress;
    void*     ClientIdProcess;
    void*     ClientIdThread;
    ULONG_PTR AffinityMask;
    LONG      Priority;
    LONG      BasePriority;
};
using NtQueryInformationThreadFn = LONG(NTAPI*)(HANDLE, int, void*, ULONG, ULONG*);

long tlsIndex() {
    auto const  base = reinterpret_cast<std::byte*>(GetModuleHandleW(nullptr));
    auto const* dos  = reinterpret_cast<IMAGE_DOS_HEADER const*>(base);
    auto const* nt   = reinterpret_cast<IMAGE_NT_HEADERS64 const*>(base + dos->e_lfanew);
    auto const& dir  = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
    if (!dir.VirtualAddress || !dir.Size) return -1;
    auto const* tls = reinterpret_cast<IMAGE_TLS_DIRECTORY64 const*>(base + dir.VirtualAddress);
    return static_cast<long>(*reinterpret_cast<DWORD const*>(tls->AddressOfIndex));
}

void** tlsArray(void* teb) { return *reinterpret_cast<void***>(static_cast<std::byte*>(teb) + kTebTls); }

class Borrowed final : public Adopted {
public:
    Borrowed(HANDLE thread, void** slot, void* own) : mThread(thread), mSlot(slot), mOwn(own) {}
    ~Borrowed() override {
        *mSlot = mOwn;
        ResumeThread(mThread);
        CloseHandle(mThread);
    }

private:
    HANDLE mThread;
    void** mSlot;
    void*  mOwn;
};

}

std::unique_ptr<Adopted> adopt(std::function<int()> const& score, std::string& note) {
    int const ownScore = score();
    long const index    = tlsIndex();
    auto const query = reinterpret_cast<NtQueryInformationThreadFn>(GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationThread"));
    if (index < 0 || !query) {
        note = "no static TLS to adopt";
        return nullptr;
    }
    void** const slot = &tlsArray(NtCurrentTeb())[index];
    void* const  own  = *slot;

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) {
        note = "cannot list threads";
        return nullptr;
    }
    THREADENTRY32 entry{sizeof entry};
    int           tried = 0, bestScore = ownScore;
    DWORD         bestId = 0;
    for (BOOL more = Thread32First(snap, &entry); more; more = Thread32Next(snap, &entry)) {
        if (entry.th32OwnerProcessID != GetCurrentProcessId() || entry.th32ThreadID == GetCurrentThreadId()) continue;
        HANDLE thread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION, FALSE, entry.th32ThreadID);
        if (!thread) continue;
        ThreadBasicInformation info{};
        if (query(thread, 0, &info, sizeof info, nullptr) < 0 || !info.TebBaseAddress || SuspendThread(thread) == DWORD(-1)) {
            CloseHandle(thread);
            continue;
        }
        void** theirs = tlsArray(info.TebBaseAddress);
        void*  block  = theirs ? theirs[index] : nullptr;
        if (block && block != own) {
            ++tried;
            *slot   = block;
            int got = score();
            *slot   = own;
            if (got > bestScore) bestScore = got, bestId = entry.th32ThreadID;
        }
        ResumeThread(thread);
        CloseHandle(thread);
    }
    CloseHandle(snap);
    if (!bestId) {
        note = std::format("this thread's own context is the best (score {}; {} threads tried)", ownScore, tried);
        return nullptr;
    }
    // stays suspended for as long as its TLS block is borrowed
    HANDLE thread = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION, FALSE, bestId);
    ThreadBasicInformation info{};
    if (!thread || query(thread, 0, &info, sizeof info, nullptr) < 0 || SuspendThread(thread) == DWORD(-1)) {
        if (thread) CloseHandle(thread);
        note = "the best thread went away";
        return nullptr;
    }
    *slot = tlsArray(info.TebBaseAddress)[index];
    note  = std::format("adopted thread {}'s context (score {}, own {}; {} threads tried)", bestId, bestScore, ownScore, tried);
    return std::make_unique<Borrowed>(thread, slot, own);
}

}
