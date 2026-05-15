#pragma once

#include <exception>
#include <string>
#include <type_traits>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/validate.hpp"
#include "pb/runtime/result.hpp"

namespace pb::runtime {

struct sequential {};

namespace detail {

template <class Stage>
[[nodiscard]] auto stage_id_for() -> stage_id {
  return stage_id{.name = std::string{pb::core::stage_traits<Stage>::name()}};
}

[[nodiscard]] inline auto has_stage_id(const stage_id& stage) noexcept -> bool {
  return !stage.key.empty() || !stage.name.empty();
}

template <class Stage, class Error>
[[nodiscard]] auto annotate_stage_error(Error&& source) {
  using ErrorType = std::remove_cvref_t<Error>;
  if constexpr (std::same_as<ErrorType, error>) {
    auto annotated = std::forward<Error>(source);
    if (!has_stage_id(annotated.stage)) {
      annotated.stage = stage_id_for<Stage>();
    }
    return annotated;
  } else {
    return std::forward<Error>(source);
  }
}

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

template <class Stage>
[[nodiscard]] auto exception_error(const std::exception& exception) -> error {
  return error{.stage = stage_id_for<Stage>(),
               .category = error_category::exception,
               .message = exception.what()};
}

template <class Stage>
[[nodiscard]] auto unknown_exception_error() -> error {
  return error{.stage = stage_id_for<Stage>(),
               .category = error_category::exception,
               .message = "stage threw an unknown exception"};
}

template <class Stage, class StageResult>
[[nodiscard]] auto normalize_stage_error(StageResult&& stage_result) -> error {
  auto normalized_error = detail::normalize_expected_error(std::forward<StageResult>(stage_result).error());
  return annotate_stage_error<Stage>(std::move(normalized_error));
}

template <class FinalOutput, class Input>
[[nodiscard]] auto try_run_after_result(Input&& input) -> result<FinalOutput> {
  return result<FinalOutput>{std::forward<Input>(input)};
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto try_run_after_result(Input&& input) -> result<FinalOutput> {
  try {
    auto stage_result = Stage{}(std::forward<Input>(input));
    if constexpr (expected_like<decltype(stage_result)>) {
      auto normalized_result = to_result(std::move(stage_result));
      if (!normalized_result.has_value()) {
        return result<FinalOutput>{normalize_stage_error<Stage>(std::move(normalized_result))};
      }
      if constexpr (sizeof...(Rest) == 0) {
        return result<FinalOutput>{std::move(normalized_result).value()};
      } else {
        return try_run_after_result<FinalOutput, decltype(std::move(normalized_result).value()), Rest...>(
            std::move(normalized_result).value());
      }
    } else if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput>{std::move(stage_result)};
    } else {
      return try_run_after_result<FinalOutput, decltype(stage_result), Rest...>(std::move(stage_result));
    }
  } catch (const std::exception& exception) {
    return result<FinalOutput>{exception_error<Stage>(exception)};
  } catch (...) {
    return result<FinalOutput>{unknown_exception_error<Stage>()};
  }
}

template <class FinalOutput, class Error, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_after_result(Input&& input) -> result<FinalOutput, Error> {
  auto stage_result = Stage{}(std::forward<Input>(input));
  if constexpr (expected_like<decltype(stage_result)>) {
    auto normalized_result = to_result(std::move(stage_result));
    if (!normalized_result.has_value()) {
      return result<FinalOutput, Error>{
          convert_error<Error>(annotate_stage_error<Stage>(std::move(normalized_result).error()))};
    }
    if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput, Error>{std::move(normalized_result).value()};
    } else {
      return run_after_result<FinalOutput, Error, decltype(std::move(normalized_result).value()), Rest...>(
          std::move(normalized_result).value());
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
  if constexpr (expected_like<decltype(stage_result)>) {
    auto normalized_result = to_result(std::move(stage_result));
    using StageResult = std::remove_cvref_t<decltype(normalized_result)>;
    using Error = typename StageResult::error_type;
    if (!normalized_result.has_value()) {
      return result<FinalOutput, Error>{annotate_stage_error<Stage>(std::move(normalized_result).error())};
    }
    if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput, Error>{std::move(normalized_result).value()};
    } else {
      return run_after_result<FinalOutput, Error, decltype(std::move(normalized_result).value()), Rest...>(
          std::move(normalized_result).value());
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

  [[nodiscard]] auto try_run(Input input) const -> result<Output> {
    if constexpr (sizeof...(Stages) == 0) {
      return result<Output>{std::move(input)};
    } else {
      return try_run_after_result<Output, Input, Stages...>(std::move(input));
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
