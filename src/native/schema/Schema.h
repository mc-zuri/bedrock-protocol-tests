#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

// STL members are built with the same MSVC STL the game uses, so the game's destructors free them.
namespace bpx::schema {

struct Value;
using List   = std::vector<Value>;
using Object = std::vector<std::pair<std::string, Value>>;

// Empty (monostate) leaves the field at its default.
struct Value {
    std::variant<std::monostate, std::int64_t, double, bool, std::string, List, Object> v;

    Value() = default;
    Value(std::int64_t x) : v(x) {}
    Value(int x) : v(std::int64_t{x}) {}
    Value(double x) : v(x) {}
    Value(bool x) : v(x) {}
    Value(char const* s) : v(std::string(s)) {}
    Value(std::string s) : v(std::move(s)) {}
    Value(List l) : v(std::move(l)) {}
    Value(Object o) : v(std::move(o)) {}

    [[nodiscard]] bool empty() const { return std::holds_alternative<std::monostate>(v); }
};

enum class Kind : std::uint8_t {
    U8, I8, Bool, U16, I16, U32, I32, U64, I64, F32, F64,
    String,
    Struct,
    Vector,
    Optional,
    Variant,
    Pointer, // std::unique_ptr to a non-polymorphic element
};

struct Field {
    std::string        name;
    Kind               kind   = Kind::U8;
    std::size_t        offset = 0;
    std::vector<Field> members;          // Vector / Optional / Pointer: one element field at offset 0
    std::size_t        elementSize = 0;
    std::size_t        flagOffset  = 0;  // Optional: engaged flag; Variant: index byte
};

using Layout = std::vector<Field>;

inline Field u8(std::string n, std::size_t o) { return {std::move(n), Kind::U8, o}; }
inline Field i8(std::string n, std::size_t o) { return {std::move(n), Kind::I8, o}; }
inline Field boolean(std::string n, std::size_t o) { return {std::move(n), Kind::Bool, o}; }
inline Field u16(std::string n, std::size_t o) { return {std::move(n), Kind::U16, o}; }
inline Field i16(std::string n, std::size_t o) { return {std::move(n), Kind::I16, o}; }
inline Field u32(std::string n, std::size_t o) { return {std::move(n), Kind::U32, o}; }
inline Field i32(std::string n, std::size_t o) { return {std::move(n), Kind::I32, o}; }
inline Field u64(std::string n, std::size_t o) { return {std::move(n), Kind::U64, o}; }
inline Field i64(std::string n, std::size_t o) { return {std::move(n), Kind::I64, o}; }
inline Field f32(std::string n, std::size_t o) { return {std::move(n), Kind::F32, o}; }
inline Field f64(std::string n, std::size_t o) { return {std::move(n), Kind::F64, o}; }
inline Field string(std::string n, std::size_t o) { return {std::move(n), Kind::String, o}; }

inline Field structure(std::string n, std::size_t o, std::vector<Field> members) {
    Field f{std::move(n), Kind::Struct, o};
    f.members = std::move(members);
    return f;
}
inline Field vec3(std::string n, std::size_t o) { return structure(std::move(n), o, {f32("x", 0), f32("y", 4), f32("z", 8)}); }
inline Field vec2(std::string n, std::size_t o) { return structure(std::move(n), o, {f32("x", 0), f32("y", 4)}); }
inline Field blockPos(std::string n, std::size_t o) { return structure(std::move(n), o, {i32("x", 0), i32("y", 4), i32("z", 8)}); }
inline Field uuid(std::string n, std::size_t o) { return structure(std::move(n), o, {u64("high", 0), u64("low", 8)}); }

inline Field vector(std::string n, std::size_t o, std::size_t elementSize, Field element) {
    Field f{std::move(n), Kind::Vector, o};
    element.offset = 0;
    f.members      = {std::move(element)};
    f.elementSize  = elementSize;
    return f;
}
// MSVC: the engaged flag follows the element's storage.
inline Field optional(std::string n, std::size_t o, std::size_t elementSize, Field element) {
    Field f{std::move(n), Kind::Optional, o};
    element.offset = 0;
    f.members      = {std::move(element)};
    f.flagOffset   = elementSize;
    return f;
}

// MSVC: storage first, index byte after the largest alternative. Value: {{"index", i}, {"value", Object{...}}}.
inline Field variant(std::string n, std::size_t o, std::size_t indexOffset, std::vector<Field> alternatives) {
    Field f{std::move(n), Kind::Variant, o};
    for (auto& a : alternatives) a.offset = 0;
    f.members    = std::move(alternatives);
    f.flagOffset = indexOffset;
    return f;
}

inline Field pointer(std::string n, std::size_t o, std::size_t elementSize, Field element) {
    Field f{std::move(n), Kind::Pointer, o};
    element.offset = 0;
    f.members      = {std::move(element)};
    f.elementSize  = elementSize;
    return f;
}

bool set(std::byte* base, Field const& field, Value const& value);

Value const& get(Object const& object, std::string_view name);

std::string toJson(Value const& value);

}
