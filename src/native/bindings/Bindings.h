#pragma once

#include "core/Version.h"

#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

// Each step (bindings/Steps.cpp) states only what changed since the previous one; load() applies
// steps oldest-first up to the running build, so unmentioned bindings carry over.
namespace bpx::bindings {

enum class Resolve {
    Start,
    Call, // match starts with `E8 rel32`
    Lea,  // match starts with `lea r64, [rip+disp32]` (7 bytes)
};

class Registry {
public:
    void function(std::string_view name, std::string_view pattern, Resolve how = Resolve::Start);
    void data(std::string_view name, std::string_view pattern) { function(name, pattern, Resolve::Lea); }
    void drop(std::string_view name);
    void slot(std::string_view name, int index);
    void offset(std::string_view name, std::int64_t value);
    void variant(std::string_view name, int value);

    // Returns the names that did not resolve to exactly one address.
    std::vector<std::string> resolve();

    [[nodiscard]] std::uintptr_t address(std::string_view name) const;   // 0 when unknown
    [[nodiscard]] int            slot(std::string_view name) const;      // -1 when unknown
    [[nodiscard]] std::int64_t   offset(std::string_view name, std::int64_t fallback = -1) const;
    [[nodiscard]] int            variant(std::string_view name, int fallback = 0) const;

    [[nodiscard]] std::map<std::string, std::string> describe() const;

    std::string currentStep;

private:
    struct Function {
        std::string    pattern;
        Resolve        how = Resolve::Start;
        std::string    step;
        std::uintptr_t address = 0;
    };
    std::map<std::string, Function, std::less<>>     mFunctions;
    std::map<std::string, int, std::less<>>          mSlots;
    std::map<std::string, std::int64_t, std::less<>> mOffsets;
    std::map<std::string, int, std::less<>>          mVariants;
};

using StepFn = void (*)(Registry&);

struct Step {
    Version     since;
    char const* name;
    StepFn      apply;
};

int addStep(char const* since, StepFn fn);

Registry& load(Version running);

Registry& current();

}

#define BPX_BINDINGS_CAT2(a, b) a##b
#define BPX_BINDINGS_CAT(a, b)  BPX_BINDINGS_CAT2(a, b)
#define BPX_BINDINGS(since)                                                                              \
    static void BPX_BINDINGS_CAT(bpxStep_, __LINE__)(::bpx::bindings::Registry & r);                    \
    [[maybe_unused]] static int const BPX_BINDINGS_CAT(bpxStepReg_, __LINE__) =                          \
        ::bpx::bindings::addStep(since, &BPX_BINDINGS_CAT(bpxStep_, __LINE__));                          \
    static void BPX_BINDINGS_CAT(bpxStep_, __LINE__)([[maybe_unused]] ::bpx::bindings::Registry & r)
