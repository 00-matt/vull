#pragma once

#include <vull/container/Array.hh>
#include <vull/support/Span.hh>
#include <vull/support/StringView.hh>
#include <vull/support/Utility.hh>

#include <stddef.h>
#include <stdint.h>

namespace vull {

template <typename T>
inline constexpr bool is_enum = __is_enum(T);

template <typename T>
concept Enum = is_enum<T>;

template <Enum E>
using underlying_type = __underlying_type(E);

template <Enum E>
constexpr auto to_underlying(E value) {
    return static_cast<underlying_type<E>>(value);
}

template <Enum E, E V>
consteval StringView enum_name_long() {
    constexpr StringView name(static_cast<const char *>(__PRETTY_FUNCTION__));
    for (size_t i = 0; i < name.length() - 3; i++) {
        if (name[i] == 'V' && name[i + 1] == ' ' && name[i + 2] == '=') {
            return {name.data() + i + 4, name.length() - i - 5};
        }
    }
    return {};
}

template <Enum E, E V, unsigned L = 1>
consteval StringView enum_name() {
    constexpr auto name = enum_name_long<E, V>();
    unsigned count = 0;
    for (size_t i = name.length(); i > 1; i--) {
        if (name[i - 1] == ':' && name[i - 2] == ':' && ++count == L) {
            return {name.data() + i, name.length() - i};
        }
    }
    return name;
}

template <Enum E, unsigned L, uint32_t... Is>
consteval auto enum_names(IntegerSequence<uint32_t, Is...>) {
    return Array<StringView, sizeof...(Is)>{{enum_name<E, static_cast<E>(Is), L>()...}};
}

template <Enum E, unsigned L>
consteval auto enum_names() {
    return enum_names<E, L>(make_integer_sequence<uint32_t, 128>());
}

template <unsigned L = 2, Enum E>
constexpr auto enum_name(E value) {
    return enum_names<E, L>()[uint32_t(value)];
}

} // namespace vull

// NOLINTBEGIN(bugprone-macro-parentheses)
#define VULL_DEFINE_FLAG_ENUM_OPS(type)                                                                                \
    inline constexpr type operator&(type lhs, type rhs) {                                                              \
        return static_cast<type>(vull::to_underlying(lhs) & vull::to_underlying(rhs));                                 \
    }                                                                                                                  \
    inline constexpr type operator|(type lhs, type rhs) {                                                              \
        return static_cast<type>(vull::to_underlying(lhs) | vull::to_underlying(rhs));                                 \
    }                                                                                                                  \
    inline constexpr type operator~(type flags) {                                                                      \
        return static_cast<type>(~vull::to_underlying(flags));                                                         \
    }
// NOLINTEND(bugprone-macro-parentheses)
