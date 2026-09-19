#pragma once

#include <cstdint>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

// Some values make the game fail fast (uncatchable), so each scenario is named in at.txt before it runs;
// the relaunched server skips finished ones and records the one named in at.txt as a crash.
namespace bpx::exporter {

struct Result {
    // Skipped: an earlier value of the same field crashed the server
    enum Kind { Ok, Partial, ReadFail, WriteFail, Crash, Skipped };
    std::string   key; // "<packet name> <scenario label>"
    Kind          kind = Ok;
    std::uint32_t id   = 0;
    std::string   body;
    std::string   detail; // Partial: the re-written bytes; ReadFail: "read N of M"
};

class Progress {
public:
    explicit Progress(std::filesystem::path dir);
    ~Progress();

    [[nodiscard]] bool done(std::string const& key) const { return mDone.contains(key); }
    void begin(std::string const& key, std::uint32_t id);
    void finish(Result const& r);
    void complete();
    [[nodiscard]] std::vector<Result> results() const;
    [[nodiscard]] std::size_t        crashes() const { return mCrashes; }
    [[nodiscard]] bool crashedUnder(std::string const& prefix) const;

private:
    void append(Result const& r);

    std::filesystem::path mDir;
    std::set<std::string> mDone, mCrashed;
    std::FILE*            mAt      = nullptr;
    std::size_t           mCrashes = 0;
};

}
