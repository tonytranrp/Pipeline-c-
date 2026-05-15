#pragma once

#include <type_traits>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/validate.hpp"
#include "pb/runtime/result.hpp"

namespace pb::runtime {

struct sequential {};

namespace detail {

template <class T>
decltype(auto) unwrap_runtime_value(T&& value) {
  if constexpr (is_result_v<T>) {
    return std::forward<T>(value).value();
  } else {
    return std::forward<T>(value);
  }
}

template <class Input>
decltype(auto) run_stages(Input&& input) {
  return std::forward<Input>(input);
}

template <class Input, class Stage, class... Rest>
decltype(auto) run_stages(Input&& input) {
  auto stage_result = Stage{}(std::forward<Input>(input));
  if constexpr (sizeof...(Rest) == 0) {
    return stage_result;
  } else {
    return run_stages<decltype(unwrap_runtime_value(std::move(stage_result))), Rest...>(
        unwrap_runtime_value(std::move(stage_result)));
  }
}

template <class Pipeline>
class sequential_engine;

template <class Input, class Output, class... Stages>
class sequential_engine<pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>>> {
public:
  [[nodiscard]] decltype(auto) run(Input input) const {
    if constexpr (sizeof...(Stages) == 0) {
      return input;
    } else {
      return run_stages<Input, Stages...>(std::move(input));
    }
  }
};

} // namespace detail

template <pb::core::ValidPipeline Pipeline>
[[nodiscard]] constexpr auto compile(sequential) {
  return detail::sequential_engine<Pipeline>{};
}

} // namespace pb::runtime

namespace pb {
using runtime::compile;
} // namespace pb
