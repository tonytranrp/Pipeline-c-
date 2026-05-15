#pragma once

#include <concepts>
#include <string_view>
#include <type_traits>

#include "pb/core/fixed_string.hpp"

namespace pb::core {

template <typename T, typename = void>
struct stage_traits {
    using name = fixed_string<0>;
    using input_type = void;
    using output_type = void;
    static constexpr bool is_valid_stage = false;
};

template <typename T>
struct stage_traits<T, std::void_t<typename T::input_type, typename T::output_type>> {
    using input_type = typename T::input_type;
    using output_type = typename T::output_type;
    static constexpr bool is_valid_stage = true;

    static constexpr auto name = []() {
        if constexpr (requires { T::name; }) {
            return T::name;
        } else if constexpr (requires { T::stage_name; }) {
            return T::stage_name;
        } else {
            return fixed_string<0>{};
        }
    }();
};

template <typename T>
inline constexpr bool is_stage_v = stage_traits<T>::is_valid_stage;

template <typename T>
using stage_input_t = typename stage_traits<T>::input_type;

template <typename T>
using stage_output_t = typename stage_traits<T>::output_type;

}  // namespace pb::core
