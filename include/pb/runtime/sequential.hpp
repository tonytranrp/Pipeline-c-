#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/validate.hpp"
#include "pb/runtime/error.hpp"
#include "pb/runtime/observer.hpp"
#include "pb/runtime/result.hpp"

namespace pb::runtime {

struct sequential {};

namespace detail {
template <class T>
struct is_result_type : std::false_type {};

template <class T, class E>
struct is_result_type<result<T, E>> : std::true_type {
  using error_type = E;
};

template <class Stage>
[[nodiscard]] auto stage_id_for(std::size_t stage_index = 0) -> stage_id {
  const auto stage_key = pb::core::stage_traits<Stage>::key();
  if (stage_key != "<unnamed>") {
    return stage_id{.key = std::string{stage_key}, .name = std::string{pb::core::stage_traits<Stage>::name()}};
  }
  std::ostringstream stream;
  stream << stage_index;
  return stage_id{.key = stream.str(), .name = std::string{pb::core::stage_traits<Stage>::name()}};
}

template <class Stage>
[[nodiscard]] auto stage_identity(std::size_t stage_index = 0) -> stage_id {
  return stage_id_for<Stage>(stage_index);
}

template <class Stage, class Error>
[[nodiscard]] auto observer_error_for(std::size_t stage_index, const Error& value, error_category fallback_category)
    -> error {
  using ErrorType = std::remove_cvref_t<Error>;
  if constexpr (std::same_as<ErrorType, error>) {
    auto annotated = value;
    if (!has_stage(annotated.stage)) {
      annotated.stage = stage_id_for<Stage>(stage_index);
    }
    return annotated;
  } else if constexpr (requires(const ErrorType& source) {
                         { source.diagnostic } -> std::convertible_to<error>;
                       }) {
    auto annotated = error{value.diagnostic};
    if (!has_stage(annotated.stage)) {
      annotated.stage = stage_id_for<Stage>(stage_index);
    }
    if (!has_message(annotated)) {
      annotated.message = detail::error_message_from(value);
    }
    return annotated;
  } else {
    return error{.stage = stage_id_for<Stage>(stage_index),
                 .category = fallback_category,
                 .message = detail::error_message_from(value)};
  }
}

template <class Stage>
void notify_stage_start(observer* sink, std::size_t stage_index) {
  if (sink != nullptr) {
    sink->on_stage_start(stage_identity<Stage>(stage_index));
  }
}

template <class Stage>
void notify_stage_success(observer* sink, std::size_t stage_index) {
  if (sink != nullptr) {
    sink->on_stage_success(stage_identity<Stage>(stage_index));
  }
}

template <class Stage, class Error>
void notify_stage_failure(observer* sink, std::size_t stage_index, const Error& value) {
  if (sink != nullptr) {
    sink->on_stage_failure(stage_identity<Stage>(stage_index),
                           observer_error_for<Stage>(stage_index, value, error_category::stage_failure));
  }
}

template <class Stage, class Error>
void notify_stage_exception(observer* sink, std::size_t stage_index, const Error& value) {
  if (sink != nullptr) {
    sink->on_stage_exception(stage_identity<Stage>(stage_index),
                             observer_error_for<Stage>(stage_index, value, error_category::exception));
  }
}

template <class Stage, class Error>
[[nodiscard]] auto annotate_stage_error(std::size_t stage_index, Error&& source) {
  using ErrorType = std::remove_cvref_t<Error>;
  if constexpr (std::same_as<ErrorType, error>) {
    auto annotated = std::forward<Error>(source);
    if (!has_stage(annotated.stage)) {
      annotated.stage = stage_id_for<Stage>(stage_index);
    }
    return annotated;
  } else {
    return std::forward<Error>(source);
  }
}

template <class Stage, class Error>
[[nodiscard]] auto annotate_stage_error(Error&& source) {
  return annotate_stage_error<Stage>(0, std::forward<Error>(source));
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
[[nodiscard]] auto exception_error(std::size_t stage_index, const std::exception& exception) -> error {
  return error{.stage = stage_id_for<Stage>(stage_index),
               .category = error_category::exception,
               .message = exception.what()};
}

template <class Stage>
[[nodiscard]] auto unknown_exception_error(std::size_t stage_index) -> error {
  return error{.stage = stage_id_for<Stage>(stage_index),
               .category = error_category::exception,
               .message = "stage threw an unknown exception"};
}

template <class Stage, class StageResult>
[[nodiscard]] auto normalize_stage_error(std::size_t stage_index, StageResult&& stage_result) -> error {
  auto normalized_error = detail::normalize_expected_error(std::forward<StageResult>(stage_result).error());
  return annotate_stage_error<Stage>(stage_index, std::move(normalized_error));
}

template <class Stage, class StageResult>
[[nodiscard]] auto normalize_stage_error(StageResult&& stage_result) -> error {
  return normalize_stage_error<Stage>(0, std::move(stage_result));
}

template <class FinalOutput, class Input>
[[nodiscard]] auto try_run_after_result(std::size_t, observer*, Input&& input) -> result<FinalOutput> {
  return result<FinalOutput>{std::forward<Input>(input)};
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto try_run_after_result(std::size_t stage_index, observer* sink, Input&& input) -> result<FinalOutput> {
  try {
    notify_stage_start<Stage>(sink, stage_index);
    auto stage_result = Stage{}(std::forward<Input>(input));
    if constexpr (expected_like<decltype(stage_result)>) {
      auto normalized_result = to_result(std::move(stage_result));
      if (!normalized_result.has_value()) {
        auto normalized_error = normalize_stage_error<Stage>(stage_index, std::move(normalized_result));
        notify_stage_failure<Stage>(sink, stage_index, normalized_error);
        return result<FinalOutput>{std::move(normalized_error)};
      }
      notify_stage_success<Stage>(sink, stage_index);
      if constexpr (sizeof...(Rest) == 0) {
        return result<FinalOutput>{std::move(normalized_result).value()};
      } else {
        return try_run_after_result<FinalOutput, decltype(std::move(normalized_result).value()), Rest...>(
            stage_index + 1, sink, std::move(normalized_result).value());
      }
    } else if constexpr (sizeof...(Rest) == 0) {
      notify_stage_success<Stage>(sink, stage_index);
      return result<FinalOutput>{std::move(stage_result)};
    } else {
      notify_stage_success<Stage>(sink, stage_index);
      return try_run_after_result<FinalOutput, decltype(stage_result), Rest...>(
          stage_index + 1, sink, std::move(stage_result));
    }
  } catch (const std::exception& exception) {
    auto stage_error = exception_error<Stage>(stage_index, exception);
    notify_stage_exception<Stage>(sink, stage_index, stage_error);
    return result<FinalOutput>{std::move(stage_error)};
  } catch (...) {
    auto stage_error = unknown_exception_error<Stage>(stage_index);
    notify_stage_exception<Stage>(sink, stage_index, stage_error);
    return result<FinalOutput>{std::move(stage_error)};
  }
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto try_run_after_result(observer* sink, Input&& input) -> result<FinalOutput> {
  return try_run_after_result<FinalOutput, Input, Stage, Rest...>(0, sink, std::forward<Input>(input));
}

template <class FinalOutput, class Error, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_after_result(std::size_t stage_index, observer* sink, Input&& input) -> result<FinalOutput, Error> {
  try {
    notify_stage_start<Stage>(sink, stage_index);
    auto stage_result = Stage{}(std::forward<Input>(input));
    if constexpr (expected_like<decltype(stage_result)>) {
      auto normalized_result = to_result(std::move(stage_result));
      if (!normalized_result.has_value()) {
        auto normalized_error =
            convert_error<Error>(annotate_stage_error<Stage>(stage_index, std::move(normalized_result).error()));
        notify_stage_failure<Stage>(sink, stage_index, normalized_error);
        return result<FinalOutput, Error>{std::move(normalized_error)};
      }
      notify_stage_success<Stage>(sink, stage_index);
      if constexpr (sizeof...(Rest) == 0) {
        return result<FinalOutput, Error>{std::move(normalized_result).value()};
      } else {
        return run_after_result<FinalOutput, Error, decltype(std::move(normalized_result).value()), Rest...>(
            stage_index + 1, sink, std::move(normalized_result).value());
      }
    } else if constexpr (sizeof...(Rest) == 0) {
      notify_stage_success<Stage>(sink, stage_index);
      return result<FinalOutput, Error>{std::move(stage_result)};
    } else {
      notify_stage_success<Stage>(sink, stage_index);
      return run_after_result<FinalOutput, Error, decltype(stage_result), Rest...>(
          stage_index + 1, sink, std::move(stage_result));
    }
  } catch (const std::exception& exception) {
    auto stage_error = convert_error<Error>(exception_error<Stage>(stage_index, exception));
    notify_stage_exception<Stage>(sink, stage_index, stage_error);
    return result<FinalOutput, Error>{std::move(stage_error)};
  } catch (...) {
    auto stage_error = convert_error<Error>(unknown_exception_error<Stage>(stage_index));
    notify_stage_exception<Stage>(sink, stage_index, stage_error);
    return result<FinalOutput, Error>{std::move(stage_error)};
  }
}

template <class FinalOutput, class Error, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_after_result(observer* sink, Input&& input) -> result<FinalOutput, Error> {
  return run_after_result<FinalOutput, Error, Input, Stage, Rest...>(0, sink, std::forward<Input>(input));
}

template <class FinalOutput, class Input>
[[nodiscard]] auto run_stages(std::size_t, observer*, Input&& input) {
  return std::forward<Input>(input);
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_stages(std::size_t stage_index, observer* sink, Input&& input) {
  auto stage_result = [&] {
    try {
      notify_stage_start<Stage>(sink, stage_index);
      return Stage{}(std::forward<Input>(input));
    } catch (const std::exception& exception) {
      auto stage_error = exception_error<Stage>(stage_index, exception);
      notify_stage_exception<Stage>(sink, stage_index, stage_error);
      throw;
    } catch (...) {
      auto stage_error = unknown_exception_error<Stage>(stage_index);
      notify_stage_exception<Stage>(sink, stage_index, stage_error);
      throw;
    }
  }();

  if constexpr (expected_like<decltype(stage_result)>) {
    auto normalized_result = to_result(std::move(stage_result));
    using StageResult = std::remove_cvref_t<decltype(normalized_result)>;
    using Error = typename StageResult::error_type;
    if (!normalized_result.has_value()) {
      auto stage_error = annotate_stage_error<Stage>(stage_index, std::move(normalized_result).error());
      notify_stage_failure<Stage>(sink, stage_index, stage_error);
      return result<FinalOutput, Error>{std::move(stage_error)};
    }
    notify_stage_success<Stage>(sink, stage_index);
    if constexpr (sizeof...(Rest) == 0) {
      return result<FinalOutput, Error>{std::move(normalized_result).value()};
    } else {
      return run_after_result<FinalOutput, Error, decltype(std::move(normalized_result).value()), Rest...>(
          stage_index + 1, sink, std::move(normalized_result).value());
    }
  } else if constexpr (sizeof...(Rest) == 0) {
    notify_stage_success<Stage>(sink, stage_index);
    return stage_result;
  } else {
    notify_stage_success<Stage>(sink, stage_index);
    return run_stages<FinalOutput, decltype(stage_result), Rest...>(stage_index + 1, sink, std::move(stage_result));
  }
}

template <class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_stages(observer* sink, Input&& input) -> decltype(auto) {
  return run_stages<FinalOutput, Input, Stage, Rest...>(0, sink, std::forward<Input>(input));
}

template <class Pipeline>
class sequential_engine;

template <class Input, class Output, class... Stages>
class sequential_engine<pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>>> {
public:
  using pipeline_type = pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>>;
  using input_type = Input;
  using output_type = Output;
  using stages = pb::meta::type_list<Stages...>;
  using try_result_type = result<Output>;

  static constexpr auto stage_count = sizeof...(Stages);

  void set_observer(observer* value) noexcept { observer_ = value; }

  [[nodiscard]] observer* get_observer() const noexcept { return observer_; }

  [[nodiscard]] decltype(auto) run(Input input) const {
    if constexpr (sizeof...(Stages) == 0) {
      return input;
    } else if constexpr (
        [] {
          using RunStagesResult = decltype(detail::run_stages<Output, Input, Stages...>(nullptr, std::declval<Input>()));
          return detail::is_result_type<RunStagesResult>::value;
        }()) {
      using RunStagesResult =
          decltype(detail::run_stages<Output, Input, Stages...>(std::declval<observer*>(), std::declval<Input>()));
      return detail::run_after_result<Output, typename detail::is_result_type<RunStagesResult>::error_type, Input, Stages...>(
          observer_, std::move(input));
    } else {
      return run_stages<Output, Input, Stages...>(observer_, std::move(input));
    }
  }

  [[nodiscard]] auto try_run(Input input) const -> result<Output> {
    if constexpr (sizeof...(Stages) == 0) {
      return result<Output>{std::move(input)};
    } else {
      return try_run_after_result<Output, Input, Stages...>(observer_, std::move(input));
    }
  }

private:
  observer* observer_{nullptr};
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
