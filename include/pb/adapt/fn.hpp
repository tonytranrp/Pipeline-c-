#pragma once

#include <concepts>
#include <functional>
#include <string_view>
#include <type_traits>
#include <utility>

#include "pb/core/concepts.hpp"

namespace pb {

namespace adapt_detail {
struct unnamed_stage {
  static constexpr std::string_view value{"unnamed"};
};

template <class Tag>
concept named_tag = requires {
  { Tag::value } -> std::convertible_to<std::string_view>;
};

template <class Tag>
constexpr std::string_view tag_value() noexcept {
  if constexpr (named_tag<Tag>) {
    return std::string_view{Tag::value};
  } else {
    return std::string_view{"unnamed"};
  }
}
} // namespace adapt_detail

template <class Tag>
struct name {
  using tag_type = Tag;
};

template <auto Function>
struct fn {
  static constexpr auto value = Function;
};

template <class Functor>
struct functor {
  using type = Functor;
};

template <class T>
struct in {
  using type = T;
};

template <class T>
struct out {
  using type = T;
};

template <class T = no_error>
struct err {
  using type = T;
};

template <class Name, class Callable, class Input, class Output, class Error = no_error>
struct callable_stage {
  using name_tag = Name;
  using input_type = Input;
  using output_type = Output;
  using error_type = Error;
  using callable_type = Callable;

  static constexpr std::string_view name = adapt_detail::tag_value<name_tag>();

  static constexpr std::string_view stage_name() noexcept { return name; }

  constexpr decltype(auto) operator()(input_type input) const
      noexcept(noexcept(std::invoke(callable_type{}, std::move(input))))
      requires std::default_initializable<callable_type> && std::invocable<callable_type, input_type>
  {
    return std::invoke(callable_type{}, std::move(input));
  }
};

template <class Name, auto Function, class Input, class Output, class Error = no_error>
struct function_stage {
  using name_tag = Name;
  using input_type = Input;
  using output_type = Output;
  using error_type = Error;
  using function_type = decltype(Function);

  static constexpr auto function = Function;
  static constexpr std::string_view name = adapt_detail::tag_value<name_tag>();

  static constexpr std::string_view stage_name() noexcept { return name; }

  constexpr decltype(auto) operator()(input_type input) const
      noexcept(noexcept(std::invoke(function, std::move(input))))
      requires std::invocable<function_type, input_type>
  {
    return std::invoke(function, std::move(input));
  }
};

template <class... Options>
struct adapt;

template <class NameTag, auto Function, class Input, class Output, class Error>
struct adapt<name<NameTag>, fn<Function>, in<Input>, out<Output>, err<Error>>
    : function_stage<NameTag, Function, Input, Output, Error> {};

template <class NameTag, auto Function, class Input, class Output>
struct adapt<name<NameTag>, fn<Function>, in<Input>, out<Output>>
    : function_stage<NameTag, Function, Input, Output> {};

template <auto Function, class Input, class Output, class Error>
struct adapt<fn<Function>, in<Input>, out<Output>, err<Error>>
    : function_stage<adapt_detail::unnamed_stage, Function, Input, Output, Error> {};

template <auto Function, class Input, class Output>
struct adapt<fn<Function>, in<Input>, out<Output>>
    : function_stage<adapt_detail::unnamed_stage, Function, Input, Output> {};

template <class NameTag, class Functor, class Input, class Output, class Error>
struct adapt<name<NameTag>, functor<Functor>, in<Input>, out<Output>, err<Error>>
    : callable_stage<NameTag, Functor, Input, Output, Error> {};

template <class NameTag, class Functor, class Input, class Output>
struct adapt<name<NameTag>, functor<Functor>, in<Input>, out<Output>>
    : callable_stage<NameTag, Functor, Input, Output> {};

template <class Functor, class Input, class Output, class Error>
struct adapt<functor<Functor>, in<Input>, out<Output>, err<Error>>
    : callable_stage<adapt_detail::unnamed_stage, Functor, Input, Output, Error> {};

template <class Functor, class Input, class Output>
struct adapt<functor<Functor>, in<Input>, out<Output>>
    : callable_stage<adapt_detail::unnamed_stage, Functor, Input, Output> {};

template <auto Function, class Input, class Output, class Error = no_error,
          class NameTag = adapt_detail::unnamed_stage>
using adapt_fn = adapt<name<NameTag>, fn<Function>, in<Input>, out<Output>, err<Error>>;

template <class Functor, class Input, class Output, class Error = no_error,
          class NameTag = adapt_detail::unnamed_stage>
using adapt_functor = adapt<name<NameTag>, functor<Functor>, in<Input>, out<Output>, err<Error>>;

template <class Stage>
concept adapted_stage = pb::core::Stage<Stage> && requires(Stage stage, typename Stage::input_type input) {
  { Stage::stage_name() } -> std::convertible_to<std::string_view>;
  stage(std::move(input));
};

} // namespace pb
