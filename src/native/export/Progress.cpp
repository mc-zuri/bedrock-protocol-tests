#include "export/Progress.h"

#include "core/Log.h"
#include "core/Watchdog.h"

#include <cerrno>
#include <cstdio>
#include <fstream>
#include <sstream>

namespace bpx::exporter {

namespace {

// results.tsv fields are hex so keys and bodies never contain tabs or newlines.
char const* const kKinds[] = {"ok", "partial", "read-fail", "write-fail", "crash", "skipped"};

std::string hex(std::string const& s) {
    static char const digits[] = "0123456789abcdef";
    std::string       out;
    out.reserve(s.size() * 2);
    for (unsigned char c : s) {
        out.push_back(digits[c >> 4]);
        out.push_back(digits[c & 15]);
    }
    return out;
}

std::string unhex(std::string const& s) {
    auto nibble = [](char c) { return c <= '9' ? c - '0' : c - 'a' + 10; };
    std::string out;
    out.reserve(s.size() / 2);
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) out.push_back(static_cast<char>(nibble(s[i]) << 4 | nibble(s[i + 1])));
    return out;
}

bool parse(std::string const& line, Result& r) {
    std::istringstream in(line);
    std::string        kind, id, key, body, detail;
    if (!std::getline(in, kind, '\t') || !std::getline(in, id, '\t') || !std::getline(in, key, '\t')) return false;
    std::getline(in, body, '\t');
    std::getline(in, detail);
    int k = -1;
    for (int i = 0; i < 6; ++i)
        if (kind == kKinds[i]) k = i;
    if (k < 0) return false;
    r = {unhex(key), static_cast<Result::Kind>(k), static_cast<std::uint32_t>(std::stoul(id)), unhex(body), unhex(detail)};
    return true;
}

}

Progress::Progress(std::filesystem::path dir) : mDir(std::move(dir)) {
    std::filesystem::create_directories(mDir);
    std::vector<Result> have = results();
    if (!have.empty()) log::info("export: {} scenarios already done", have.size());
    for (auto const& r : have) {
        mDone.insert(r.key);
        if (r.kind == Result::Crash) ++mCrashes, mCrashed.insert(r.key);
    }
    std::ifstream at(mDir / "at.txt");
    std::string   id, key;
    if (std::getline(at, id) && std::getline(at, key) && !key.empty() && !mDone.contains(unhex(key))) {
        Result crash{unhex(key), Result::Crash, static_cast<std::uint32_t>(std::stoul(id)), {}};
        append(crash);
        mDone.insert(crash.key);
        mCrashed.insert(crash.key);
        ++mCrashes;
        log::info("export: the last run died on \"{}\"; recorded as a crash, going on after it", crash.key);
    }
    at.close();
    mAt = std::fopen((mDir / "at.txt").string().c_str(), "wb");
    if (!mAt) log::info("export: cannot open {} (errno {})", (mDir / "at.txt").string(), errno);
}

Progress::~Progress() {
    if (mAt) std::fclose(mAt);
}

void Progress::complete() {
    // Not in the destructor: a game fault may unwind through the export, and at.txt must survive it.
    if (mAt) std::fclose(mAt);
    mAt = nullptr;
    std::error_code ec;
    std::filesystem::remove(mDir / "at.txt", ec);
}

void Progress::begin(std::string const& key, std::uint32_t id) {
    watchdog::arm("export " + key);
    if (!mAt) return;
    std::string line = std::to_string(id) + "\n" + hex(key) + "\n";
    std::fseek(mAt, 0, SEEK_SET);
    std::fwrite(line.data(), 1, line.size(), mAt);
    std::fflush(mAt); // not truncated: a stale tail after the second line is never read
}

void Progress::finish(Result const& r) {
    watchdog::disarm();
    append(r);
    mDone.insert(r.key);
}

void Progress::append(Result const& r) {
    std::ofstream out(mDir / "results.tsv", std::ios::app | std::ios::binary);
    out << kKinds[r.kind] << '\t' << r.id << '\t' << hex(r.key) << '\t' << hex(r.body) << '\t' << hex(r.detail) << '\n';
}

bool Progress::crashedUnder(std::string const& prefix) const {
    auto it = mCrashed.lower_bound(prefix);
    return it != mCrashed.end() && it->starts_with(prefix);
}

std::vector<Result> Progress::results() const {
    std::vector<Result> out;
    std::ifstream       in(mDir / "results.tsv", std::ios::binary);
    for (std::string line; std::getline(in, line);) {
        Result r;
        if (parse(line, r)) out.push_back(std::move(r));
    }
    return out;
}

}
