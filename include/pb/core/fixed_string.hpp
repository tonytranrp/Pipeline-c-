#pragma once

#include <array>
#include <cstddef>
#include <string_view>

namespace pb::core {

template <std::size_t N>
struct fixed_string {
    std::array<char, N + 1> value{};

    constexpr fixed_string() = default;

    constexpr explicit fixed_string(const char (&str)[N + 1]) {
        for (std::size_t i = 0; i < N + 1; ++i) {
            value[i] = str[i];
        }
    }

    constexpr std::size_t size() const noexcept { return N; }

    constexpr const char* c_str() const noexcept { return value.data(); }

    constexpr std::string_view view() const noexcept {
        return {value.data(), N};
    }

    constexpr auto operator<=>(const fixed_string&) const = default;
};

template <fixed_string Str>
struct named {
    static constexpr fixed_string<Str.size()> value{Str};
};

template <std::size_t N>
constexpr fixed_string<N - 1> make_fixed_string(const char (&str)[N]) {
    return fixed_string<N - 1>(str);
}

template <std::size_t N>
fixed_string(const char (&str)[N]) -> fixed_string<N - 1>;

inline constexpr fixed_string<0> empty_name{};

}  // namespace pb::core
