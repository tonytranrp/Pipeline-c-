#pragma once

#include <type_traits>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/validate.hpp"
#include "pb/runtime/result.hpp"

namespace pb::runtime {

struct sequential {};

namespace detail {

template <class TargetError, class SourceError>
[[nodiscard]] auto convert_error(SourceError&& source) -> TargetError {
  if constexpr (std::constructible_from<TargetError, SourceError>) {
    return TargetError{std::forward<SourceError>(source)};
  } else {
    static_assert(std::same_as<TargetError, std::remove_cvref_t<SourceError>>,
                  "Pipeline stages that return result<T, E> must use a compatible error type");
  }
}

template <class FinalOutput, class Error, class Input>
[[nodiscard]] auto run_after_result(Input&& input) -> result<FinalOutput, Error> {
  return result<FinalOutput, Error>{std::forward<Input>(input)};
}

template <class FinalOutput, class Error, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_after_result(Input&& input) -> result<FinalOutput, Error> {
  auto stage_result = Stage{}(std::forward<Input>(input));
  if constexpr (is_result_v<decltype(stage_result)>) {
    if (!stage_result.has_value()) {
      return result<FinalOutput, Error>{convert_error<Error>(std::move(stage_result).error())};
    }
    if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput, Error>{std::move(stage_result).value()};
    } else {
      return run_after_result<FinalOutput, Error, decltype(std::move(stage_result).value()), Rest...>(
          std::move(stage_result).value());
    }
  } else if constexpr (sizeof...(Rest) == 0) {
    return result<FinalOutput, Error>{std::move(stage_result)};
  } else {
    return run_after_result<FinalOutput, Error, decltype(stage_result), Rest...>(std::move(stage_result));
  }
}

template <class FinalOutput, class Input>
[[nodiscard]] decltype(auto) run_stages(Input&& input) {
  return std::forward<Input>(input);
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] decltype(auto) run_stages(Input&& input) {
  auto stage_result = Stage{}(std::forward<Input>(input));
  if constexpr (is_result_v<decltype(stage_result)>) {
    using StageResult = std::remove_cvref_t<decltype(stage_result)>;
    using Error = typename StageResult::error_type;
    if (!stage_result.has_value()) {
      return result<FinalOutput, Error>{std::move(stage_result).error()};
    }
    if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput, Error>{std::move(stage_result).value()};
    } else {
      return run_after_result<FinalOutput, Error, decltype(std::move(stage_result).value()), Rest...>(
          std::move(stage_result).value());
    }
  } else if constexpr (sizeof...(Rest) == 0) {
    return stage_result;
  } else {
    return run_stages<FinalOutput, decltype(stage_result), Rest...>(std::move(stage_result));
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
      return run_stages<Output, Input, Stages...>(std::move(input));
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
