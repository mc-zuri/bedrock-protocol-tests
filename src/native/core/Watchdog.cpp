#include "core/Watchdog.h"

#include "core/Log.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

namespace bpx::watchdog {

namespace {

using Clock = std::chrono::steady_clock;

constexpr auto kLimit = std::chrono::seconds(5);

std::mutex             gMutex;
std::string            gLabel;
Clock::time_point      gSince;
std::atomic<bool>      gArmed{false};
std::once_flag         gStarted;

void watch() {
    for (;;) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        if (!gArmed) continue;
        std::string label;
        {
            std::lock_guard lock(gMutex);
            if (!gArmed || Clock::now() - gSince < kLimit) continue;
            label = gLabel;
        }
        log::info("watchdog: \"{}\" still running after {} s; ending the server (the rerun goes on after it)", label,
                  std::chrono::duration_cast<std::chrono::seconds>(kLimit).count());
        TerminateProcess(GetCurrentProcess(), 0xB9A7);
    }
}

}

void arm(std::string label) {
    std::call_once(gStarted, [] { std::thread(watch).detach(); });
    std::lock_guard lock(gMutex);
    gLabel = std::move(label);
    gSince = Clock::now();
    gArmed = true;
}

void disarm() { gArmed = false; }

}
