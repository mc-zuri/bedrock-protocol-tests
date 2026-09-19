#include "bindings/Bindings.h"

#include "memory/Scanner.h"

#include <algorithm>
#include <format>

namespace bpx::bindings {

namespace {

std::vector<Step>& steps() {
    static std::vector<Step> s;
    return s;
}

Registry& registry() {
    static Registry r;
    return r;
}

}

void Registry::function(std::string_view name, std::string_view pattern, Resolve how) {
    mFunctions.insert_or_assign(std::string(name), Function{std::string(pattern), how, currentStep, 0});
}

void Registry::drop(std::string_view name) {
    if (auto it = mFunctions.find(name); it != mFunctions.end()) mFunctions.erase(it);
    if (auto it = mSlots.find(name); it != mSlots.end()) mSlots.erase(it);
    if (auto it = mOffsets.find(name); it != mOffsets.end()) mOffsets.erase(it);
    if (auto it = mVariants.find(name); it != mVariants.end()) mVariants.erase(it);
}

void Registry::slot(std::string_view name, int index) { mSlots.insert_or_assign(std::string(name), index); }

void Registry::offset(std::string_view name, std::int64_t value) { mOffsets.insert_or_assign(std::string(name), value); }

void Registry::variant(std::string_view name, int value) { mVariants.insert_or_assign(std::string(name), value); }

std::vector<std::string> Registry::resolve() {
    auto const& game = memory::Module::game();
    std::vector<std::string> failed;
    for (auto& [name, fn] : mFunctions) {
        fn.address   = 0;
        auto pattern = memory::Pattern::parse(fn.pattern);
        if (!pattern) {
            failed.push_back(name + " (bad pattern)");
            continue;
        }
        auto hits = pattern->find(game.textBegin, game.textEnd, 2);
        if (hits.size() != 1) {
            failed.push_back(std::format("{} ({} matches)", name, hits.size()));
            continue;
        }
        switch (fn.how) {
        case Resolve::Start: fn.address = hits[0]; break;
        case Resolve::Call: fn.address = memory::relTarget(hits[0] + 1); break;
        case Resolve::Lea: fn.address = memory::relTarget(hits[0] + 3); break;
        }
    }
    return failed;
}

std::uintptr_t Registry::address(std::string_view name) const {
    auto it = mFunctions.find(name);
    return it == mFunctions.end() ? 0 : it->second.address;
}

int Registry::slot(std::string_view name) const {
    auto it = mSlots.find(name);
    return it == mSlots.end() ? -1 : it->second;
}

std::int64_t Registry::offset(std::string_view name, std::int64_t fallback) const {
    auto it = mOffsets.find(name);
    return it == mOffsets.end() ? fallback : it->second;
}

int Registry::variant(std::string_view name, int fallback) const {
    auto it = mVariants.find(name);
    return it == mVariants.end() ? fallback : it->second;
}

std::map<std::string, std::string> Registry::describe() const {
    auto const& game = memory::Module::game();
    std::map<std::string, std::string> out;
    for (auto const& [name, fn] : mFunctions)
        out[name] = fn.address ? std::format("0x{:X} (step {})", game.rva(fn.address), fn.step) : std::format("unresolved (step {})", fn.step);
    for (auto const& [name, v] : mSlots) out[name] = std::format("slot {}", v);
    for (auto const& [name, v] : mOffsets) out[name] = std::format("0x{:X}", v);
    for (auto const& [name, v] : mVariants) out[name] = std::format("variant {}", v);
    return out;
}

int addStep(char const* since, StepFn fn) {
    steps().push_back({Version::parse(since), since, fn});
    return 0;
}

Registry& load(Version running) {
    auto ordered = steps();
    std::ranges::stable_sort(ordered, {}, &Step::since);
    Registry& r = registry();
    for (auto const& step : ordered) {
        if (step.since > running) break;
        r.currentStep = step.name;
        step.apply(r);
    }
    r.currentStep.clear();
    return r;
}

Registry& current() { return registry(); }

}
