#pragma once

#include <concepts>

#include "pb/core/concepts.hpp"

namespace pb::core {

namespace detail {

template <typename From, typename... Stages>
struct pipeline_state;

template <typename ExpectedInput, typename... Stages>
struct validate_chain;

template <typename ExpectedInput>
struct validate_chain<ExpectedInput> {
    static constexpr bool value = true;
};

template <typename ExpectedInput, typename Stage>
struct validate_chain<ExpectedInput, Stage> {
    static constexpr bool value =
        stage<Stage> && std::same_as<typename stage_traits<Stage>::input_type, ExpectedInput>;
    static_assert(value,
                  "pb::core::validate: pipeline terminal stage is invalid or has mismatched input type");
};

template <typename ExpectedInput, typename First, typename Second, typename... Rest>
struct validate_chain<ExpectedInput, First, Second, Rest...> {
    static constexpr bool first_ok =
        stage<First> && std::same_as<typename stage_traits<First>::input_type, ExpectedInput>;
    static constexpr bool adjacent_ok = connectable<First, Second>;
    static constexpr bool value =
        first_ok && adjacent_ok &&
        validate_chain<typename stage_traits<Second>::input_type, Second, Rest...>::value;

    static_assert(first_ok,
                  "pb::core::validate: pipeline initial stage input does not match from<T>");
    static_assert(adjacent_ok,
                  "pb::core::validate: adjacent pipeline stages are not connectable");
};

}  // namespace detail

template <typename State>
struct validate;

template <typename From, typename... Stages>
struct validate<detail::pipeline_state<From, Stages...>> {
    static constexpr bool value = detail::validate_chain<From, Stages...>::value;
};

template <typename ExpectedInput, typename... Stages>
struct validate_chain : detail::validate_chain<ExpectedInput, Stages...> {};

template <typename ExpectedInput, typename... Stages>
inline constexpr bool validate_chain_v = validate_chain<ExpectedInput, Stages...>::value;

template <typename State>
inline constexpr bool is_valid_chain_v = validate<State>::value;

}  // namespace pb::core
