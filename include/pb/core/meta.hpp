#pragma once

#include <type_traits>

namespace pb::core {

namespace meta {

template <typename... Ts>
struct type_list {};

template <typename T>
struct is_type_list : std::false_type {};

template <typename... Ts>
struct is_type_list<type_list<Ts...>> : std::true_type {};

template <typename T>
inline constexpr bool is_type_list_v = is_type_list<T>::value;

template <std::size_t N>
using size_constant = std::integral_constant<std::size_t, N>;

using npos = size_constant<static_cast<std::size_t>(-1)>;

template <typename... Ts>
struct type_list_size;
template <typename... Ts>
struct type_list_size<type_list<Ts...>> : size_constant<sizeof...(Ts)> {};

template <typename T>
inline constexpr auto type_list_size_v = type_list_size<T>::value;

template <typename List, typename T>
struct contains : std::false_type {};

template <typename... Ts, typename T>
struct contains<type_list<Ts...>, T> : std::bool_constant<(std::is_same_v<Ts, T> || ...)> {};

template <typename List>
struct front;

template <typename Head, typename... Tail>
struct front<type_list<Head, Tail...>> { using type = Head; };

template <typename List>
using front_t = typename front<List>::type;

template <typename List>
struct pop_front;

template <typename Head, typename... Tail>
struct pop_front<type_list<Head, Tail...>> { using type = type_list<Tail...>; };

template <typename List>
using pop_front_t = typename pop_front<List>::type;

template <typename... Lists>
struct concat;

template <typename... Ts>
struct concat<type_list<Ts...>> { using type = type_list<Ts...>; };

template <typename... Ts, typename... Us, typename... Rest>
struct concat<type_list<Ts...>, type_list<Us...>, Rest...> {
    using type = typename concat<type_list<Ts..., Us...>, Rest...>::type;
};

template <typename List>
struct reverse_impl;

template <>
struct reverse_impl<type_list<>> { using type = type_list<>; };

template <typename Head, typename... Tail>
struct reverse_impl<type_list<Head, Tail...>> {
    using type = typename concat<typename reverse_impl<type_list<Tail...>>::type, type_list<Head>>::type;
};

template <typename List>
using reverse_t = typename reverse_impl<List>::type;

template <typename List>
struct to_array;

namespace detail {

template <typename, std::size_t>
struct nth_type;

template <typename Head, typename... Tail, std::size_t N>
struct nth_type<type_list<Head, Tail...>, N> {
    using type = typename nth_type<type_list<Tail...>, N - 1>::type;
};

template <typename Head, typename... Tail>
struct nth_type<type_list<Head, Tail...>, 0> { using type = Head; };

}  // namespace detail

template <typename List, std::size_t N>
struct at {
    static_assert(N < type_list_size_v<List>, "meta::at index out of range");
    using type = typename detail::nth_type<List, N>::type;
};

template <typename List, std::size_t N>
using at_t = typename at<List, N>::type;

template <typename T, typename List>
struct index_of;

template <typename List>
struct index_of_impl;

namespace detail {

template <typename T, typename List, std::size_t I>
struct find_index_impl;

template <typename T, std::size_t I>
struct find_index_impl<T, type_list<>, I> : npos {};

template <typename T, std::size_t I, typename Head, typename... Tail>
struct find_index_impl<T, type_list<Head, Tail...>, I>
    : std::conditional_t<std::is_same_v<T, Head>, size_constant<I>, find_index_impl<T, type_list<Tail...>, I + 1>> {};

}  // namespace detail

template <typename T, typename List>
struct index_of : detail::find_index_impl<T, List, 0> {};

}  // namespace meta

}  // namespace pb::core
