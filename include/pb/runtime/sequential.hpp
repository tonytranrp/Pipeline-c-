#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/stage_traits.hpp"
#include "pb/core/validate.hpp"
#include "pb/runtime/result.hpp"

namespace pb::runtime {

struct sequential {};

namespace detail {

template <class StageList>
struct stage_tuple;

template <class... Stages>
struct stage_tuple<pb::meta::type_list<Stages...>> {
  using type = std::tuple<Stages...>;
};

template <class StageList>
using stage_tuple_t = typename stage_tuple<StageList>::type;

template <class Stage, class Input>
decltype(auto) invoke_stage(Input&& input) {
  return Stage{}(std::forward<Input>(input));
}

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
  auto stage_result = invoke_stage<Stage>(std::forward<Input>(input));
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
  using input_type = Input;
  using output_type = Output;
  using pipeline_type = pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>>;

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
