#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

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

template <class T, class List>
struct push_front;

template <class T, class... Ts>
struct push_front<T, type_list<Ts...>> { using type = type_list<T, Ts...>; };

template <class T, class List>
using push_front_t = typename push_front<T, List>::type;

namespace detail {

template <class List, class T, class IndexSequence>
struct replace_back_impl;

template <class... Ts, class T, std::size_t... Is>
struct replace_back_impl<type_list<Ts...>, T, std::index_sequence<Is...>> {
  using tuple_t = std::tuple<Ts...>;
  static constexpr std::size_t last = sizeof...(Ts) - 1;
  using type = type_list<std::conditional_t<Is < last, std::tuple_element_t<Is, tuple_t>, T>...>;
};

} // namespace detail

template <class List, class T>
struct replace_back;

template <class T, class U>
struct replace_back<type_list<T>, U> { using type = type_list<U>; };

template <class... Ts, class T>
struct replace_back<type_list<Ts...>, T> {
  using type = typename detail::replace_back_impl<
      type_list<Ts...>, T, std::make_index_sequence<sizeof...(Ts)>>::type;
};

template <class List, class T>
using replace_back_t = typename replace_back<List, T>::type;

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

template <class... Ts, std::size_t Index>
struct at<type_list<Ts...>, Index> {
  static_assert(Index < sizeof...(Ts), "pb::meta::at index out of range");
  using type = std::tuple_element_t<Index, std::tuple<Ts...>>;
};

template <class List, std::size_t Index>
using at_t = typename at<List, Index>::type;

template <class List>
using front_t = at_t<List, 0>;

template <class List>
using back_t = at_t<List, size_v<List> - 1>;

} // namespace pb::meta

namespace pb::core::meta {
using ::pb::meta::all_of;
using ::pb::meta::at;
using ::pb::meta::at_t;
using ::pb::meta::back_t;
using ::pb::meta::contains;
using ::pb::meta::front_t;
using ::pb::meta::push_back;
using ::pb::meta::push_back_t;
using ::pb::meta::push_front;
using ::pb::meta::push_front_t;
using ::pb::meta::replace_back;
using ::pb::meta::replace_back_t;
using ::pb::meta::size;
using ::pb::meta::size_v;
using ::pb::meta::transform;
using ::pb::meta::type_list;
} // namespace pb::core::meta
