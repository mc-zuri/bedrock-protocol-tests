// Env (set by bpx_launch): BPX_OUT, BPX_WORK, BPX_VERSION (minecraft-data), BPX_SERVER_VERSION (exact build),
// BPX_PACKETS (optional, comma-separated), BPX_GO / BPX_DONE (event names).
#include <windows.h>

#include "core/Log.h"
#include "export/Exporter.h"
#include "server/Server.h"

#include <cstdlib>
#include <string>

namespace {

std::string env(char const* name) {
    char const* v = std::getenv(name);
    return v ? v : "";
}

DWORD WINAPI worker(LPVOID) {
    std::string out = env("BPX_OUT"), work = env("BPX_WORK"), version = env("BPX_VERSION"), serverVersion = env("BPX_SERVER_VERSION");
    if (out.empty() || work.empty() || version.empty()) return 1;
    bpx::log::open(work + "\\bpx.log");
    bpx::log::info("bpx agent: {} ({})", serverVersion, version);

    HANDLE go   = OpenEventA(SYNCHRONIZE, FALSE, env("BPX_GO").c_str());
    HANDLE done = OpenEventA(EVENT_MODIFY_STATE, FALSE, env("BPX_DONE").c_str());
    if (go) {
        WaitForSingleObject(go, 10 * 60 * 1000);
        CloseHandle(go);
    }

    std::string error;
    auto        server = bpx::server::connect(bpx::Version::parse(version), error);
    if (!server) {
        bpx::log::info("cannot bind this build: {}", error);
    } else {
        bpx::log::info("bound: payload at 0x{:X}, ids up to {}", server->payloadOffset(), server->maxPacketId());
        bpx::exporter::Options options{out, work, serverVersion.empty() ? version : serverVersion, {}, version};
        std::string            list = env("BPX_PACKETS");
        for (std::size_t at = 0; at < list.size();) {
            auto comma = list.find(',', at);
            if (comma == std::string::npos) comma = list.size();
            if (comma > at) options.only.insert(list.substr(at, comma - at));
            at = comma + 1;
        }
        bpx::exporter::run(*server, options);
    }
    bpx::log::info("done");
    if (done) {
        SetEvent(done);
        CloseHandle(done);
    }
    return 0;
}

}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        if (HANDLE t = CreateThread(nullptr, 0, worker, nullptr, 0, nullptr)) CloseHandle(t);
    }
    return TRUE;
}
