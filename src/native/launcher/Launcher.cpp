// bpx_launch.exe [versions...] [--out <dir>] [--timeout <seconds>] [--jobs <n>] [--retries <n>] [--packets A,B]
// Settings (env, else <repo>/.env): SERVERS_DIR, OUT_DIR, JOBS.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <atomic>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

namespace {

struct Build {
    std::string mcDataVersion, serverVersion;
};

std::mutex gPrint, gCreate;

template <class... Args>
void say(Build const& b, char const* fmt, Args... args) {
    std::lock_guard lock(gPrint);
    std::printf("[%s] ", b.serverVersion.c_str());
    std::printf(fmt, args...);
    std::printf("\n");
    std::fflush(stdout);
}

fs::path exeDir() {
    wchar_t buf[MAX_PATH]{};
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    return fs::path(buf).parent_path();
}

fs::path findRoot() {
    for (fs::path p = exeDir(); !p.empty() && p != p.root_path(); p = p.parent_path())
        if (fs::exists(p / "versions.json")) return p;
    return {};
}

std::string readFile(fs::path const& p) {
    std::ifstream     f(p, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::string setting(fs::path const& root, char const* key, std::string const& fallback) {
    if (char const* v = std::getenv(key); v && *v) return v;
    std::istringstream in(readFile(root / ".env"));
    for (std::string line; std::getline(in, line);) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        auto eq = line.find('=');
        if (line.empty() || line[0] == '#' || eq == std::string::npos) continue;
        auto trim = [](std::string t) {
            t.erase(0, t.find_first_not_of(" \t"));
            t.erase(t.find_last_not_of(" \t") + 1);
            if (t.size() >= 2 && (t.front() == '"' || t.front() == '\'') && t.back() == t.front()) t = t.substr(1, t.size() - 2);
            return t;
        };
        if (trim(line.substr(0, eq)) == key) {
            std::string v = trim(line.substr(eq + 1));
            if (!v.empty()) return v;
        }
    }
    return fallback;
}

fs::path dirSetting(fs::path const& root, char const* key, char const* fallback) {
    fs::path p = setting(root, key, fallback);
    return fs::absolute(p.is_absolute() ? p : root / p).lexically_normal();
}

std::wstring wide(std::string const& s) { return std::wstring(s.begin(), s.end()); }

// A per-child block rather than SetEnvironmentVariable, so builds can run side by side.
std::wstring environment(std::vector<std::pair<std::wstring, std::wstring>> const& extra) {
    std::wstring block;
    wchar_t*     env = GetEnvironmentStringsW();
    for (wchar_t const* p = env; *p; p += wcslen(p) + 1) {
        std::wstring kv(p);
        bool overridden = std::any_of(extra.begin(), extra.end(), [&](auto const& e) { return kv.rfind(e.first + L"=", 0) == 0; });
        if (!overridden) block.append(kv).push_back(L'\0');
    }
    FreeEnvironmentStringsW(env);
    for (auto const& [k, v] : extra) block.append(k).append(L"=").append(v).push_back(L'\0');
    block.push_back(L'\0');
    return block;
}

bool inject(HANDLE process, fs::path const& dll) {
    std::string path   = dll.string();
    auto        size   = path.size() + 1;
    LPVOID      remote = VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remote || !WriteProcessMemory(process, remote, path.c_str(), size, nullptr)) return false;
    auto loadLibrary = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));
    bool loaded      = false;
    for (int attempt = 0; attempt < 8 && !loaded; ++attempt) {
        HANDLE t = CreateRemoteThread(process, nullptr, 0, loadLibrary, remote, 0, nullptr);
        if (!t) {
            Sleep(250);
            continue;
        }
        WaitForSingleObject(t, 15000);
        DWORD code = 0;
        GetExitCodeThread(t, &code);
        CloseHandle(t);
        loaded = code != 0;
        if (!loaded) Sleep(250);
    }
    VirtualFreeEx(process, remote, 0, MEM_RELEASE);
    return loaded;
}

// Died: the server went down or timed out, so a rerun can resume after it.
enum class Outcome { Ok, Errors, Died };

Outcome run(Build const& b, fs::path const& serversDir, fs::path const& outRoot, fs::path const& agent, int timeoutSec, std::string const& packets,
         fs::path const& captures) {
    fs::path serverDir = serversDir / b.mcDataVersion;
    fs::path exe       = serverDir / "bedrock_server.exe";
    fs::path out       = outRoot / b.mcDataVersion / "generated-packets.bin";
    fs::path work      = outRoot / b.mcDataVersion / ".export";
    if (!fs::exists(exe)) {
        say(b, "FAILED: no %s (run `pnpm servers`)", exe.string().c_str());
        return Outcome::Errors;
    }
    if (!fs::exists(captures / b.mcDataVersion / "captured-packets.bin")) {
        say(b, "FAILED: no captured packets (run `pnpm capture`)");
        return Outcome::Errors;
    }
    if (fs::exists(work / "result.txt")) fs::remove_all(work);
    fs::create_directories(work);
    auto const t0 = std::chrono::steady_clock::now();

    std::string  goName   = "Local\\bpx_go_" + std::to_string(GetCurrentProcessId()) + "_" + b.mcDataVersion;
    std::string  doneName = "Local\\bpx_done_" + std::to_string(GetCurrentProcessId()) + "_" + b.mcDataVersion;
    HANDLE       go       = CreateEventA(nullptr, TRUE, FALSE, goName.c_str());
    HANDLE       done     = CreateEventA(nullptr, TRUE, FALSE, doneName.c_str());
    std::wstring env      = environment({{L"BPX_OUT", out.wstring()},
                                         {L"BPX_WORK", work.wstring()},
                                         {L"BPX_VERSION", wide(b.mcDataVersion)},
                                         {L"BPX_SERVER_VERSION", wide(b.serverVersion)},
                                         {L"BPX_GO", wide(goName)},
                                         {L"BPX_DONE", wide(doneName)},
                                         {L"BPX_PACKETS", wide(packets)},
                                         {L"BPX_CAPTURES", captures.wstring()}});

    // Serialized: the pipe ends are inheritable, and a server started in parallel must not inherit them.
    std::unique_lock    creating(gCreate);
    SECURITY_ATTRIBUTES sa{sizeof sa, nullptr, TRUE};
    HANDLE              outRd = nullptr, outWr = nullptr, inRd = nullptr, inWr = nullptr;
    CreatePipe(&outRd, &outWr, &sa, 0);
    CreatePipe(&inRd, &inWr, &sa, 0);
    SetHandleInformation(outRd, HANDLE_FLAG_INHERIT, 0);
    SetHandleInformation(inWr, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
    si.cb         = sizeof si;
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdOutput = outWr;
    si.hStdError  = outWr;
    si.hStdInput  = inRd;
    PROCESS_INFORMATION pi{};
    std::wstring        cmd = L"\"" + exe.wstring() + L"\"";
    BOOL ok = CreateProcessW(exe.c_str(), cmd.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW | CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT,
                             env.data(), serverDir.c_str(), &si, &pi);
    CloseHandle(outWr);
    CloseHandle(inRd);
    creating.unlock();
    if (!ok) {
        say(b, "FAILED: CreateProcess: %lu", GetLastError());
        return Outcome::Died;
    }
    ResumeThread(pi.hThread);
    Sleep(200); // the loader must be up for LoadLibrary; inject() retries while it is not
    if (!inject(pi.hProcess, agent)) {
        say(b, "FAILED: could not inject the agent");
        TerminateProcess(pi.hProcess, 1);
        return Outcome::Died;
    }

    std::atomic<bool> started{false};
    std::thread       reader([&] {
        // not inheritable: another build's server must not hold it open
        HANDLE log = CreateFileW((work / "server.log").c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
        char          buf[4096];
        DWORD         n = 0;
        std::string   tail;
        while (ReadFile(outRd, buf, sizeof buf, &n, nullptr) && n) {
            DWORD written = 0;
            WriteFile(log, buf, n, &written, nullptr);
            tail.append(buf, n);
            if (!started && tail.find("Server started") != std::string::npos) {
                started = true;
                SetEvent(go);
            }
            if (tail.size() > 8192) tail.erase(0, tail.size() - 1024);
        }
        CloseHandle(log);
    });

    HANDLE waits[2] = {done, pi.hProcess};
    DWORD  w        = WaitForMultipleObjects(2, waits, FALSE, static_cast<DWORD>(timeoutSec) * 1000);
    bool   finished = w == WAIT_OBJECT_0;

    // No `stop`: the world is not needed, and saving it only costs time.
    TerminateProcess(pi.hProcess, 0);
    WaitForSingleObject(pi.hProcess, 10000);
    CloseHandle(inWr);
    // A crash reporter the server spawned can keep the pipe's write end open, blocking ReadFile forever.
    Sleep(100);
    CancelSynchronousIo(reader.native_handle());
    reader.join();
    CloseHandle(outRd);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(go);
    CloseHandle(done);
    double const seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - t0).count();
    std::string  result  = readFile(work / "result.txt");
    std::string  logs    = (work / "bpx.log").string();
    if (!finished) {
        say(b, "FAILED: %s after %.1f s, see %s", w == WAIT_OBJECT_0 + 1 ? "the server went down" : "timed out", seconds, logs.c_str());
        return Outcome::Died;
    }
    if (result.empty()) {
        say(b, "FAILED: nothing exported (%.1f s), see %s", seconds, logs.c_str());
        return Outcome::Errors;
    }
    if (result.find("NOT") != std::string::npos) {
        say(b, "FAILED: %s (%.1f s), see %s", result.c_str(), seconds, logs.c_str());
        return Outcome::Errors;
    }
    std::error_code ec;
    fs::remove_all(work, ec);
    say(b, "ok: %s (%.1f s)", result.c_str(), seconds);
    return Outcome::Ok;
}

}

int main(int argc, char** argv) {
    // Inherited by every server: a crash ends it at once instead of waiting on a WER / fault dialog.
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
    fs::path root = findRoot();
    if (root.empty()) {
        std::fprintf(stderr, "no versions.json above %s\n", exeDir().string().c_str());
        return 2;
    }
    fs::path    serversDir = dirSetting(root, "SERVERS_DIR", "servers");
    fs::path    outDir     = dirSetting(root, "OUT_DIR", "out");
    fs::path    outRoot    = outDir;
    fs::path    captures   = outDir; // not moved by --out
    int         jobs       = std::max(1, std::atoi(setting(root, "JOBS", "4").c_str()));
    int         timeout    = 600;
    int         retries    = 0;
    std::string packets;
    std::vector<std::string> only;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--out" && i + 1 < argc) outRoot = fs::absolute(argv[++i]);
        else if (a == "--timeout" && i + 1 < argc) timeout = std::atoi(argv[++i]);
        else if (a == "--jobs" && i + 1 < argc) jobs = std::max(1, std::atoi(argv[++i]));
        else if (a == "--retries" && i + 1 < argc) retries = std::max(0, std::atoi(argv[++i]));
        else if (a == "--packets" && i + 1 < argc) packets = argv[++i];
        else if (a.rfind("--", 0) == 0) {
            std::fprintf(stderr, "unknown option %s\n", a.c_str());
            return 2;
        } else only.push_back(a);
    }
    std::printf("servers: %s\nout:     %s\njobs:    %d\n", serversDir.string().c_str(), outRoot.string().c_str(), jobs);
    std::fflush(stdout);

    std::string json = readFile(root / "versions.json");
    std::vector<Build> todo;
    std::regex         entry(R"re("mcDataVersion"\s*:\s*"([^"]+)"\s*,\s*"serverVersion"\s*:\s*"([^"]+)")re");
    for (auto it = std::sregex_iterator(json.begin(), json.end(), entry); it != std::sregex_iterator(); ++it) {
        Build b{(*it)[1].str(), (*it)[2].str()};
        if (only.empty() || std::find(only.begin(), only.end(), b.mcDataVersion) != only.end()) todo.push_back(b);
    }

    fs::path                 agent = exeDir() / "bpx_agent.dll";
    std::atomic<std::size_t> next{0};
    std::vector<std::string> failed;
    auto                     worker = [&] {
        for (std::size_t i; (i = next++) < todo.size();) {
            // Fail fast by default: a crash is a packet-def bug to fix, not something to restart past.
            Outcome outcome = Outcome::Died;
            for (int attempt = 0; attempt <= retries && outcome == Outcome::Died; ++attempt)
                outcome = run(todo[i], serversDir, outRoot, agent, timeout, packets, captures);
            if (outcome != Outcome::Ok) {
                std::lock_guard lock(gPrint);
                failed.push_back(todo[i].mcDataVersion);
            }
        }
    };
    std::vector<std::thread> pool;
    for (int j = 0; j < jobs; ++j) pool.emplace_back(worker);
    for (auto& t : pool) t.join();
    std::printf("exported %zu/%zu builds without an error\n", todo.size() - failed.size(), todo.size());
    if (!failed.empty()) {
        std::printf("failed:");
        for (auto const& v : failed) std::printf(" %s", v.c_str());
        std::printf("\n");
    }
    return failed.empty() ? 0 : 1;
}
