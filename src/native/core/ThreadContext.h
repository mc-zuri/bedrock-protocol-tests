#pragma once

#include <functional>
#include <memory>
#include <string>

namespace bpx::threads {

// Item stack (de)serialization uses ItemRegistryManager's thread_local registry scope, which only game
// threads have. adopt() points this thread's static TLS slot at the best-scoring game thread's block,
// keeping that thread suspended until the result is destroyed.
class Adopted {
public:
    virtual ~Adopted() = default;
};
std::unique_ptr<Adopted> adopt(std::function<int()> const& score, std::string& note);

}
