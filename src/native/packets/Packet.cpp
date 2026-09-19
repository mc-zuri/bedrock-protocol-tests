#include "packets/Packet.h"

#include <algorithm>

namespace bpx::packets {

PacketDef& PacketDef::shape(std::string_view since, Layout layout) {
    mShapes.emplace_back(Version::parse(since), std::move(layout));
    std::ranges::stable_sort(mShapes, {}, &std::pair<Version, Layout>::first);
    return *this;
}

PacketDef& PacketDef::values(std::string_view field, List domain) { return values("0.0.0", field, std::move(domain)); }

PacketDef& PacketDef::values(std::string_view since, std::string_view field, List domain) {
    mValues.push_back({Version::parse(since), std::string(field), std::move(domain)});
    return *this;
}

PacketDef& PacketDef::baseline(std::string_view field, Value value) { return baseline("0.0.0", field, std::move(value)); }

PacketDef& PacketDef::baseline(std::string_view since, std::string_view field, Value value) {
    mBaselines.push_back({Version::parse(since), std::string(field), std::move(value)});
    return *this;
}

PacketDef& PacketDef::scenario(std::string label, Object values) { return scenario("0.0.0", std::move(label), std::move(values)); }

PacketDef& PacketDef::scenario(std::string_view since, std::string label, Object values) {
    return scenario(since, "9999.0.0", std::move(label), std::move(values));
}

PacketDef& PacketDef::scenario(std::string_view since, std::string_view until, std::string label, Object values) {
    mExtra.push_back({Version::parse(since), Version::parse(until), {std::move(label), std::move(values)}});
    return *this;
}

PacketDef& PacketDef::payload(std::string label, std::string bytes) { return payload("0.0.0", "9999.0.0", std::move(label), std::move(bytes)); }

PacketDef& PacketDef::payload(std::string_view since, std::string_view until, std::string label, std::string bytes) {
    mPayloads.push_back({Version::parse(since), Version::parse(until), std::move(label), std::move(bytes), {}, 0});
    return *this;
}

PacketDef& PacketDef::captured(std::string capture, std::size_t skip) {
    mPayloads.push_back({Version::parse("0.0.0"), Version::parse("9999.0.0"), "captured:" + capture, {}, capture, skip});
    return *this;
}

std::vector<PacketDef::Payload> PacketDef::payloads(Version v) const {
    std::vector<Payload> out;
    for (auto const& p : mPayloads)
        if (p.since <= v && v < p.until) out.push_back(p);
    return out;
}

PacketDef& PacketDef::checkedVia(std::string_view since, std::string_view until, std::string packet, std::size_t own, std::size_t other,
                                 std::size_t size, std::string capture) {
    mVia.push_back({Version::parse(since), Version::parse(until), std::move(packet), own, other, size, std::move(capture)});
    return *this;
}

PacketDef::Via const* PacketDef::via(Version v) const {
    for (auto const& x : mVia)
        if (x.since <= v && v < x.until) return &x;
    return nullptr;
}

PacketDef& PacketDef::rewrites(std::string why) {
    mRewrites = std::move(why);
    return *this;
}

PacketDef& PacketDef::since(std::string_view version) {
    mSince = Version::parse(version);
    return *this;
}

PacketDef& PacketDef::until(std::string_view version) {
    mUntil = Version::parse(version);
    return *this;
}

PacketDef& PacketDef::unnamed(std::uint32_t id) {
    mUnnamed = id;
    return *this;
}

Layout const* PacketDef::layout(Version v) const {
    Layout const* out = nullptr;
    for (auto const& [since, layout] : mShapes) {
        if (since > v) break;
        out = &layout;
    }
    return out;
}

bool PacketDef::supports(Version v) const { return mSince <= v && v < mUntil; }

std::vector<Scenario> PacketDef::scenarios(Layout const& layout, Version v) const {
    auto inLayout = [&](std::string const& name) {
        return std::ranges::any_of(layout, [&](Field const& f) { return f.name == name; });
    };
    std::vector<std::pair<std::string, List const*>> domains;
    for (auto const& d : mValues) {
        if (d.since > v || !inLayout(d.field)) continue;
        auto it = std::ranges::find_if(domains, [&](auto const& kv) { return kv.first == d.field; });
        if (it == domains.end()) domains.emplace_back(d.field, &d.values);
        else it->second = &d.values; // declared in version order, so the newest wins
    }
    Object base;
    for (auto const& b : mBaselines) {
        if (b.since > v || !inLayout(b.field)) continue;
        auto it = std::ranges::find_if(base, [&](auto const& kv) { return kv.first == b.field; });
        if (it == base.end()) base.emplace_back(b.field, b.value);
        else it->second = b.value;
    }
    auto with = [&](Object const& own) {
        Object o = own;
        for (auto const& [k, value] : base)
            if (std::ranges::none_of(own, [&](auto const& kv) { return kv.first == k; })) o.emplace_back(k, value);
        return o;
    };

    // One field per scenario, so a member that rejects a value only costs its own sweep.
    std::vector<Scenario> out;
    out.push_back({"default", with({})});
    for (auto const& [field, domain] : domains)
        for (std::size_t i = 0; i < domain->size(); ++i)
            out.push_back({field + "[" + std::to_string(i) + "]", with(Object{{field, (*domain)[i]}})});
    // Only where the layout has every field an extra names, or it would set a dependent member without its flag.
    for (auto const& [since, until, extra] : mExtra)
        if (since <= v && v < until && std::ranges::all_of(extra.values, [&](auto const& kv) { return inLayout(kv.first); }))
            out.push_back({extra.label, with(extra.values)});
    return out;
}

std::map<std::string, PacketDef, std::less<>>& all() {
    static std::map<std::string, PacketDef, std::less<>> defs;
    return defs;
}

int add(std::string name, void (*define)(PacketDef&)) {
    auto [it, _] = all().try_emplace(name, name);
    define(it->second);
    return 0;
}

}
