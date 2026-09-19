#include "export/Exporter.h"

#include "bindings/Bindings.h"
#include "core/Log.h"
#include "core/ThreadContext.h"
#include "core/Watchdog.h"
#include "export/Dump.h"
#include "export/Progress.h"
#include "memory/Guard.h"
#include "packets/Captures.h"
#include "packets/Packet.h"

#include <Windows.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <climits>
#include <cstring>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <optional>

namespace bpx::exporter {

namespace {

using server::GamePacket;
using server::PacketId;
using server::Server;

void save(std::filesystem::path const& path, std::string const& body) {
    std::ofstream f(path, std::ios::binary);
    f.write(body.data(), static_cast<std::streamsize>(body.size()));
}

enum class RoundTrip { Ok, Partial, Fail, NoReader };

struct Checked {
    RoundTrip   how = RoundTrip::Fail;
    std::string detail; // Partial: the re-written bytes; Fail: how far the reader got
};

// Pointer members (e.g. a live actor the writer dereferences) are not on the wire, so a packet
// read back gets them re-applied from the def's baselines.
struct Surroundings {
    schema::Layout const* layout = nullptr;
    schema::Object        values;
    void                  apply(Server& s, GamePacket const& p) const {
        if (!layout) return;
        std::byte* fields = p.data() + s.payloadOffset();
        memory::guard([&] {
            for (auto const& f : *layout)
                if (f.kind == schema::Kind::Pointer) schema::set(fields, f, schema::get(values, f.name));
        });
    }
};

Checked roundTrip(Server& s, PacketId id, std::string const& body, Surroundings const& around = {}) {
    GamePacket fresh = s.create(id);
    if (!fresh) return {RoundTrip::Fail, "no packet"};
    around.apply(s, fresh);
    auto r = s.read(fresh, body);
    if (!r) {
        auto const f = memory::lastFault();
        return {RoundTrip::Fail, std::format("reader faulted: exception {:08x} at {}{:x}", f.code, f.inImage ? "rva " : "", f.rva)};
    }
    auto where = [&] { return r->consumed ? std::format("read {} of {}", *r->consumed, body.size()) : std::string(); };
    if (!r->ok) return {RoundTrip::Fail, "rejected; " + where()};
    if (r->consumed && *r->consumed != body.size()) return {RoundTrip::Fail, "accepted without reading all of it; " + where()};
    auto again = s.write(fresh);
    if (!again) return {RoundTrip::Fail, "writing what was read faulted"};
    if (*again != body) return {RoundTrip::Partial, *again};
    return {RoundTrip::Ok, {}};
}

}

void run(Server& s, Options const& o) {
    std::filesystem::path const work(o.workDir);
    std::filesystem::create_directories(work);

    // Item stacks need a game thread's thread-local item registry (see threads::adopt). From 1.26.40 only
    // reading needs it, so the score counts both default writes and capture reads.
    std::string adoptNote;
    watchdog::arm("adopt a game thread's context");
    auto const joinPackets = packets::captures::all(o.version);
    auto const score       = [&] {
        int n = 0;
        for (PacketId id = 1; id <= s.maxPacketId(); ++id) {
            GamePacket probe = s.create(id);
            if (probe && s.write(probe)) ++n;
        }
        for (auto const& c : joinPackets)
            if (GamePacket probe = s.create(c.id); probe && s.read(probe, c.payload)) ++n;
        return n;
    };
    auto const adopted = threads::adopt(score, adoptNote);
    watchdog::disarm();
    log::info("thread context: {}", adoptNote);

    struct Known {
        PacketId    id;
        std::string name;
    };
    std::vector<Known> known;
    for (PacketId id = 1; id <= s.maxPacketId(); ++id)
        if (GamePacket p = s.create(id)) known.push_back({id, s.name(p)});
    log::info("factory: {} packets", known.size());

    std::string unshaped, unknown;
    std::map<std::string, PacketId, std::less<>> idByName;
    for (auto const& k : known) idByName[k.name] = k.id;
    for (auto const& [name, def] : packets::all())
        for (auto const& k : known)
            if (k.name.empty() && def.unnamedId() == k.id && !idByName.contains(name)) idByName[name] = k.id;

    Progress progress(work);
    auto const captured = packets::captures::all(o.version);

    auto const readVia = [&](std::string const& key, PacketId id, std::string const& payload, Surroundings const& around,
                             packets::PacketDef::Via const& via, bool begun = false) {
        if (!begun) progress.begin(key, id);
        auto fail = [&](Result::Kind kind, std::string detail, std::string body = {}) { progress.finish({key, kind, id, std::move(body), std::move(detail)}); };
        auto otherId  = idByName.find(via.packet);
        auto expected = std::ranges::find(captured, via.capture, &packets::captures::Capture::name);
        if (otherId == idByName.end() || expected == captured.end()) return fail(Result::ReadFail, "no " + via.packet + " / capture " + via.capture, payload);
        GamePacket p = s.create(id), q = s.create(otherId->second);
        if (!p || !q) return fail(Result::ReadFail, "no packet", payload);
        around.apply(s, p);
        auto r = s.read(p, payload);
        if (!r) return fail(Result::ReadFail, "reader faulted", payload);
        if (!r->ok) return fail(Result::ReadFail, std::format("rejected; read {} of {}", r->consumed.value_or(0), payload.size()), payload);
        if (r->consumed && *r->consumed != payload.size())
            return fail(Result::ReadFail, std::format("accepted without reading all of it; read {} of {}", *r->consumed, payload.size()), payload);
        std::byte* a = p.data() + s.payloadOffset() + via.own;
        std::byte* b = q.data() + s.payloadOffset() + via.other;
        std::swap_ranges(a, a + via.size, b); // same type on both sides, so each destructor frees what it now holds
        auto written = s.write(q);
        if (!written) return fail(Result::ReadFail, via.packet + " did not write what was read", payload);
        if (*written != expected->payload) return fail(Result::Partial, *written, payload);
        progress.finish({key, Result::Ok, id, payload,
                         std::format("read, then written by {} exactly as its {}-byte capture", via.packet, expected->payload.size())});
    };

    auto const readBack = [&](std::string const& key, PacketId id, std::string const& payload, Surroundings const& around, bool anySize = false) {
        progress.begin(key, id);
        auto        checked = roundTrip(s, id, payload, around);
        std::string body    = payload;
        // The game may re-encode what it reads (JSON, completed identifiers, hash map order). Accept its own bytes
        // if they round-trip or cycle back to a seen body, and keep >= 95% of the size unless anySize (nothing dropped).
        std::string              own = checked.detail;
        std::vector<std::string> seen{payload};
        for (int pass = 0; pass < 4 && checked.how == RoundTrip::Partial && (anySize || own.size() * 20 >= payload.size() * 19); ++pass) {
            auto again = roundTrip(s, id, own, around);
            if (again.how == RoundTrip::Ok) {
                checked = {RoundTrip::Ok, std::format("the game's writing of the {}-byte body", payload.size())};
                body    = std::move(own);
                break;
            }
            if (again.how != RoundTrip::Partial) break;
            seen.push_back(std::move(own));
            if (std::ranges::find(seen, again.detail) != seen.end()) {
                checked = {RoundTrip::Ok, std::format("read and written back in another order, cycling back after {} passes", seen.size())};
                break;
            }
            own = std::move(again.detail);
        }
        Result::Kind kind = checked.how == RoundTrip::Ok ? Result::Ok : checked.how == RoundTrip::Partial ? Result::Partial : Result::ReadFail;
        progress.finish({key, kind, id, body, checked.detail});
    };

    for (auto const& [name, def] : packets::all()) {
        // since / until only gate the field scenarios, not payloads
        auto const payloads = def.payloads(s.version());
        if ((!def.supports(s.version()) && payloads.empty()) || (!o.only.empty() && !o.only.contains(name))) continue;
        auto idIt = idByName.find(name);
        if (idIt == idByName.end()) {
            if (def.supports(s.version())) unknown += std::format("{}{}", unknown.empty() ? "" : ",", dump::quote(name));
            continue;
        }
        auto const* layout = def.supports(s.version()) ? def.layout(s.version()) : nullptr;
        if (!layout && payloads.empty()) {
            unshaped += std::format("{}{}", unshaped.empty() ? "" : ",", dump::quote(name));
            continue;
        }
        PacketId id = idIt->second;
        auto const   list = layout ? def.scenarios(*layout, s.version()) : std::vector<packets::Scenario>{};
        Surroundings around{layout, layout ? list.front().values : schema::Object{}};
        for (auto const& pl : payloads) {
            std::string key = name + " " + (pl.capture.empty() ? "payload:" + pl.label : pl.label);
            if (progress.done(key)) continue;
            std::string bytes = pl.bytes;
            if (!pl.capture.empty()) {
                auto it = std::ranges::find(captured, pl.capture, &packets::captures::Capture::name);
                if (it == captured.end()) continue;
                bytes = it->payload.substr(std::min(pl.skip, it->payload.size()));
            }
            if (auto const* via = def.via(s.version())) readVia(key, id, bytes, around, *via);
            else readBack(key, id, bytes, around, def.rewritesOwnWay());
        }
        for (auto const& sc : list) {
            std::string key = name + " " + sc.label;
            if (progress.done(key)) continue;
            if (auto bracket = sc.label.find('['); bracket != std::string::npos
                && progress.crashedUnder(name + " " + sc.label.substr(0, bracket + 1))) {
                progress.finish({key, Result::Skipped, id, {}});
                continue;
            }
            progress.begin(key, id);
            // Destroy the packet before recording the result: a bad value can crash its destructor,
            // which must count against this scenario.
            std::optional<std::string> body;
            {
                GamePacket p = s.create(id);
                if (!p) break;
                std::byte* fields = p.data() + s.payloadOffset();
                bool       set    = memory::guard([&] {
                    for (auto const& f : *layout) schema::set(fields, f, schema::get(sc.values, f.name));
                });
                if (set) body = s.write(p);
            }
            if (!body) {
                auto const f = memory::lastFault();
                progress.finish({key, Result::WriteFail, id, {},
                                 std::format("exception {:08x} at {}{:x}", f.code, f.inImage ? "rva " : "", f.rva)});
                continue;
            }
            if (auto const* via = def.via(s.version())) {
                readVia(key, id, *body, around, *via, true);
                continue;
            }
            auto         checked = roundTrip(s, id, *body, around);
            Result::Kind kind    = checked.how == RoundTrip::Ok ? Result::Ok : checked.how == RoundTrip::Partial ? Result::Partial : Result::ReadFail;
            progress.finish({key, kind, id, *body, checked.detail});
        }
    }

    std::map<PacketId, std::string> nameById;
    for (auto const& k : known) nameById[k.id] = k.name.empty() ? std::format("#{}", k.id) : k.name;
    for (auto const& [name, id] : idByName) nameById[id] = name;
    for (auto const& c : captured) {
        auto nameIt = nameById.find(c.id);
        if (nameIt == nameById.end() || (!o.only.empty() && !o.only.contains(nameIt->second))) continue;
        std::string key = nameIt->second + " captured:" + c.name;
        if (progress.done(key)) continue;
        Surroundings around;
        if (auto defIt = packets::all().find(nameIt->second); defIt != packets::all().end() && defIt->second.supports(s.version()))
            if (auto const* layout = defIt->second.layout(s.version())) around = {layout, defIt->second.scenarios(*layout, s.version()).front().values};
        readBack(key, c.id, c.payload, around);
    }
    log::info("captures: {} packets captured from this build's server ({})", captured.size(), o.version);

    for (auto const& k : known) {
        std::string const& name = nameById[k.id];
        auto defIt = packets::all().find(k.name);
        // Only a def that covers this build but has no shape for it yet falls back to the bare packet: a def that
        // rules the build out with since() / until() means the game's encoding here is not one to export.
        if (!k.name.empty() && defIt != packets::all().end()
            && (!defIt->second.supports(s.version()) || defIt->second.layout(s.version())
                || !defIt->second.payloads(s.version()).empty()))
            continue;
        if (!k.name.empty() && idByName[k.name] != k.id) continue; // a name shared by two ids
        if (k.name.empty() && std::ranges::any_of(idByName, [&](auto const& kv) { return kv.second == k.id; })) continue;
        if (!o.only.empty() && !o.only.contains(name)) continue;
        std::string key = name + " default";
        if (progress.done(key)) continue;
        progress.begin(key, k.id);
        std::optional<std::string> body;
        {
            GamePacket p = s.create(k.id);
            if (!p) break;
            body = s.write(p);
        }
        if (!body) {
            auto const f = memory::lastFault();
            progress.finish({key, Result::WriteFail, k.id, {}, std::format("exception {:08x} at {}{:x}", f.code, f.inImage ? "rva " : "", f.rva)});
            continue;
        }
        auto         checked = roundTrip(s, k.id, *body);
        Result::Kind kind    = checked.how == RoundTrip::Ok ? Result::Ok : checked.how == RoundTrip::Partial ? Result::Partial : Result::ReadFail;
        progress.finish({key, kind, k.id, *body, checked.detail});
    }

    progress.complete();

    std::map<PacketId, std::vector<Result>> byPacket;
    int ok = 0, partial = 0, readFails = 0, writeFails = 0;
    for (auto const& r : progress.results()) {
        switch (r.kind) {
        case Result::Ok: ++ok; break;
        case Result::Partial: ++partial; break;
        case Result::ReadFail:
            ++readFails;
            log::info("not read back: {} ({})", r.key, r.detail);
            continue;
        default:
            ++writeFails;
            log::info("{}: {} {}", r.kind == Result::Crash ? "took the server down" : r.kind == Result::Skipped ? "skipped" : "write failed", r.key, r.detail);
            continue;
        }
        byPacket[r.id].push_back(r);
    }
    if (!unknown.empty()) log::info("packet files this build has no packet for: {}", unknown);
    if (!unshaped.empty()) log::info("packet files without a shape for this build: {}", unshaped);

    dump::Writer file(o.serverVersion);
    for (auto const& [id, results] : byPacket)
        for (auto const& r : results) file.packet(id, std::format(R"({{"p":{}}})", dump::quote(r.key)), r.body);
    std::filesystem::path const out(o.outFile), tmp = out.string() + ".tmp";
    std::filesystem::create_directories(out.parent_path());
    save(tmp, file.bytes());
    std::filesystem::rename(tmp, out);

    std::string summary = std::format("{} packets, {} scenarios", byPacket.size(), ok + partial);
    if (partial) summary += std::format(" ({} written back the game's own way)", partial);
    if (readFails) summary += std::format("; {} NOT READ BACK", readFails);
    if (writeFails) summary += std::format("; {} NOT WRITTEN", writeFails);
    log::info("{}", summary);
    save(work / "result.txt", summary);
}

}
