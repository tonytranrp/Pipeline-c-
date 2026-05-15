#pragma once

#include <cstddef>
#include <type_traits>

namespace pb::meta {

template <class... Ts>
struct type_list {};

template <class List>
struct size;

template <class... Ts>
struct size<type_list<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template <class List>
inline constexpr std::size_t size_v = size<List>::value;

template <class List, class T>
struct push_back;

template <class... Ts, class T>
struct push_back<type_list<Ts...>, T> { using type = type_list<Ts..., T>; };

template <class List, class T>
using push_back_t = typename push_back<List, T>::type;

template <class List, class T>
struct contains;

template <class... Ts, class T>
struct contains<type_list<Ts...>, T> : std::bool_constant<(std::is_same_v<Ts, T> || ...)> {};

template <class List, template <class> class F>
struct transform;

template <class... Ts, template <class> class F>
struct transform<type_list<Ts...>, F> { using type = type_list<typename F<Ts>::type...>; };

template <class List, template <class> class Pred>
struct all_of;

template <class... Ts, template <class> class Pred>
struct all_of<type_list<Ts...>, Pred> : std::bool_constant<(Pred<Ts>::value && ...)> {};

template <class List, std::size_t Index>
struct at;

namespace detail {
template <std::size_t Index, class Head, class... Tail>
struct at_impl : at_impl<Index - 1, Tail...> {};

template <class Head, class... Tail>
struct at_impl<0, Head, Tail...> { using type = Head; };
} // namespace detail

template <class... Ts, std::size_t Index>
struct at<type_list<Ts...>, Index> {
  static_assert(Index < sizeof...(Ts), "pb::meta::at index out of range");
  using type = typename detail::at_impl<Index, Ts...>::type;
};

template <class List, std::size_t Index>
using at_t = typename at<List, Index>::type;

} // namespace pb::meta

namespace pb::core::meta {
using ::pb::meta::all_of;
using ::pb::meta::at;
using ::pb::meta::at_t;
using ::pb::meta::contains;
using ::pb::meta::push_back;
using ::pb::meta::push_back_t;
using ::pb::meta::size;
using ::pb::meta::size_v;
using ::pb::meta::transform;
using ::pb::meta::type_list;
} // namespace pb::core::meta
