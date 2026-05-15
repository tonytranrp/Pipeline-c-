#pragma once

#include <type_traits>

#include "pb/core/concepts.hpp"
#include "pb/core/meta.hpp"

namespace pb::core {

namespace detail {

template <typename...>
struct always_false : std::false_type {};

template <typename From, typename To>
struct edge_valid : std::bool_constant<connectable<From, To> || stage_to_sink<From, To> || source_to_stage<From, To>> {};

template <typename From, typename... Stages>
struct validate_chain_impl;

template <typename From>
struct validate_chain_impl<From> : std::true_type {};

template <typename From, typename Next, typename... Rest>
struct validate_chain_impl<From, Next, Rest...>
    : std::bool_constant<edge_valid<From, Next>::value && validate_chain_impl<Next, Rest...>::value> {};

}  // namespace detail

template <typename... Ts>
struct validate_chain : detail::validate_chain_impl<Ts...> {};

template <typename... Ts>
inline constexpr bool validate_chain_v = validate_chain<Ts...>::value;

template <typename... Ts>
using validated_chain = std::bool_constant<validate_chain_v<Ts...>>;

}  // namespace pb::core
