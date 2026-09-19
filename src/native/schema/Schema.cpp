#include "schema/Schema.h"

#include "export/Dump.h"

#include <cstring>
#include <format>
#include <new>

namespace bpx::schema {

namespace {

struct RawVector {
    std::byte* first;
    std::byte* last;
    std::byte* end;
};

template <class T>
bool setNumber(std::byte* at, Value const& value) {
    if (auto const* i = std::get_if<std::int64_t>(&value.v)) {
        T x = static_cast<T>(*i);
        std::memcpy(at, &x, sizeof x);
        return true;
    }
    if (auto const* d = std::get_if<double>(&value.v)) {
        T x = static_cast<T>(*d);
        std::memcpy(at, &x, sizeof x);
        return true;
    }
    if (auto const* b = std::get_if<bool>(&value.v)) {
        T x = static_cast<T>(*b ? 1 : 0);
        std::memcpy(at, &x, sizeof x);
        return true;
    }
    return false;
}

// A zeroed std::string is invalid (it needs SSO capacity 15); every other kind is valid at zero.
void construct(std::byte* at, Field const& f) {
    switch (f.kind) {
    case Kind::String: new (at) std::string(); break;
    case Kind::Struct:
        for (auto const& m : f.members) construct(at + m.offset, m);
        break;
    default: break;
    }
}

}

bool set(std::byte* base, Field const& f, Value const& value) {
    if (value.empty()) return true;
    std::byte* at = base + f.offset;
    switch (f.kind) {
    case Kind::U8: return setNumber<std::uint8_t>(at, value);
    case Kind::I8: return setNumber<std::int8_t>(at, value);
    case Kind::Bool: return setNumber<bool>(at, value);
    case Kind::U16: return setNumber<std::uint16_t>(at, value);
    case Kind::I16: return setNumber<std::int16_t>(at, value);
    case Kind::U32: return setNumber<std::uint32_t>(at, value);
    case Kind::I32: return setNumber<std::int32_t>(at, value);
    case Kind::U64: return setNumber<std::uint64_t>(at, value);
    case Kind::I64: return setNumber<std::int64_t>(at, value);
    case Kind::F32: return setNumber<float>(at, value);
    case Kind::F64: return setNumber<double>(at, value);
    case Kind::String: {
        auto const* s = std::get_if<std::string>(&value.v);
        if (!s) return false;
        *reinterpret_cast<std::string*>(at) = *s;
        return true;
    }
    case Kind::Struct: {
        auto const* o = std::get_if<Object>(&value.v);
        if (!o) return false;
        for (auto const& m : f.members)
            if (!set(at, m, get(*o, m.name))) return false;
        return true;
    }
    case Kind::Vector: {
        auto const* l = std::get_if<List>(&value.v);
        if (!l) return false;
        auto&       v    = *reinterpret_cast<RawVector*>(at);
        std::size_t size = l->size() * f.elementSize;
        // std::allocator, not malloc: large blocks carry the aligned back-pointer the game's deallocate expects
        std::byte* mem = size ? std::allocator<std::byte>().allocate(size) : nullptr;
        if (size) std::memset(mem, 0, size);
        Field const& element = f.members.front();
        for (std::size_t i = 0; i < l->size(); ++i) {
            construct(mem + i * f.elementSize, element);
            if (!set(mem + i * f.elementSize, element, (*l)[i])) return false;
        }
        // overwriting without freeing is fine: a default-constructed vector owns no buffer
        v.first = mem;
        v.last  = mem + size;
        v.end   = mem + size;
        return true;
    }
    case Kind::Variant: {
        // the default alternative owns no heap memory, so its storage can be cleared without destroying it
        auto const* o = std::get_if<Object>(&value.v);
        if (!o) return false;
        auto const* index = std::get_if<std::int64_t>(&get(*o, "index").v);
        if (!index || *index < 0 || static_cast<std::size_t>(*index) >= f.members.size()) return false;
        Field const& alt = f.members[static_cast<std::size_t>(*index)];
        std::memset(at, 0, f.flagOffset);
        construct(at, alt);
        if (!set(at, alt, get(*o, "value"))) return false;
        *reinterpret_cast<std::uint8_t*>(at + f.flagOffset) = static_cast<std::uint8_t>(*index);
        return true;
    }
    case Kind::Pointer: {
        Field const& element = f.members.front();
        auto*&       ptr     = *reinterpret_cast<std::byte**>(at);
        if (!ptr) {
            ptr = std::allocator<std::byte>().allocate(f.elementSize);
            std::memset(ptr, 0, f.elementSize);
            construct(ptr, element);
        }
        return set(ptr, element, value);
    }
    case Kind::Optional: {
        Field const& element = f.members.front();
        auto*        engaged = reinterpret_cast<bool*>(at + f.flagOffset);
        // a disengaged optional's storage is uninitialized (inner flags included)
        if (!*engaged) {
            std::memset(at, 0, f.flagOffset);
            construct(at, element);
        }
        if (!set(at, element, value)) return false;
        *engaged = true;
        return true;
    }
    }
    return false;
}

Value const& get(Object const& object, std::string_view name) {
    static Value const none;
    for (auto const& [k, v] : object)
        if (k == name) return v;
    return none;
}

std::string toJson(Value const& value) {
    return std::visit(
        [](auto const& x) -> std::string {
            using T = std::decay_t<decltype(x)>;
            if constexpr (std::is_same_v<T, std::monostate>) return "null";
            else if constexpr (std::is_same_v<T, std::int64_t>) return std::to_string(x);
            else if constexpr (std::is_same_v<T, double>) return std::format("{}", x);
            else if constexpr (std::is_same_v<T, bool>) return x ? "true" : "false";
            else if constexpr (std::is_same_v<T, std::string>) return dump::quote(x);
            else if constexpr (std::is_same_v<T, List>) {
                std::string out = "[";
                for (std::size_t i = 0; i < x.size(); ++i) out += (i ? "," : "") + toJson(x[i]);
                return out + "]";
            } else {
                std::string out = "{";
                for (std::size_t i = 0; i < x.size(); ++i) out += (i ? "," : "") + dump::quote(x[i].first) + ":" + toJson(x[i].second);
                return out + "}";
            }
        },
        value.v
    );
}

}
