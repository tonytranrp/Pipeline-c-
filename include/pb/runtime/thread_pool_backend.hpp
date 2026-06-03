#pragma once

#include <cstddef>
#include <exception>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "pb/runtime/sequential.hpp"
#include "pb/runtime/thread_pool.hpp"

namespace pb::runtime {

struct thread_pool_backend {
  std::size_t worker_count{std::thread::hardware_concurrency()};
};

namespace policy {
namespace scheduling {
struct deterministic_case_order {};
} // namespace scheduling

namespace cancellation {
struct drain_running_cases {};
} // namespace cancellation
} // namespace policy

namespace detail_thread_pool {

class synchronized_observer final : public observer {
public:
  explicit synchronized_observer(observer* sink) noexcept : sink_{sink} {}

  void on_stage_start(const stage_id& id) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_stage_start(id);
  }

  void on_stage_success(const stage_id& id) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_stage_success(id);
  }

  void on_stage_failure(const stage_id& id, const error& diagnostic) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_stage_failure(id, diagnostic);
  }

  void on_stage_exception(const stage_id& id, const error& diagnostic) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_stage_exception(id, diagnostic);
  }

  void on_case_selected(const stage_id& branch_id, std::size_t case_index, const stage_id& predicate_id) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_case_selected(branch_id, case_index, predicate_id);
  }

  void on_case_skipped(const stage_id& branch_id, std::size_t case_index, const stage_id& predicate_id) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_case_skipped(branch_id, case_index, predicate_id);
  }

  void on_case_failed(const stage_id& branch_id, std::size_t case_index, const stage_id& case_stage_id,
                      const error& diagnostic) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_case_failed(branch_id, case_index, case_stage_id, diagnostic);
  }

  void on_fan_in_started(const stage_id& branch_id, std::size_t case_count) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_fan_in_started(branch_id, case_count);
  }

  void on_fan_in_case_scheduled(const stage_id& branch_id, std::size_t case_index,
                                const stage_id& case_stage_id) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_fan_in_case_scheduled(branch_id, case_index, case_stage_id);
  }

  void on_fan_in_case_completed(const stage_id& branch_id, std::size_t case_index, const stage_id& case_stage_id,
                                bool success) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_fan_in_case_completed(branch_id, case_index, case_stage_id, success);
  }

  void on_fan_in_completed(const stage_id& branch_id, std::size_t selected_count, std::size_t completed_count,
                           std::size_t failed_count) override {
    if (!sink_) return;
    const std::scoped_lock lock{mutex_};
    sink_->on_fan_in_completed(branch_id, selected_count, completed_count, failed_count);
  }

private:
  observer* sink_{};
  std::mutex mutex_{};
};

template <class BranchNode, class BranchStage, class BranchStageObject, std::size_t StageIndex,
          std::size_t CaseIndex, class Input, class Aggregate>
[[nodiscard]] auto enqueue_fan_in_case(thread_pool& pool, BranchStageObject&& branch_stage, synchronized_observer& sink,
                                       const stage_id& branch_id, const Input& input, Aggregate& aggregate) {
  auto branch_stage_id = detail::branch_child_stage_id<BranchNode, BranchStage>(StageIndex, CaseIndex, "stage");
  if constexpr (std::copy_constructible<Input>) {
    return pool.enqueue([stage = std::forward<BranchStageObject>(branch_stage), &sink, branch_id, branch_stage_id,
                         &aggregate, input_copy = Input{input}]() mutable {
      auto stage_result = detail::run_fan_in_case_stage<BranchNode, BranchStage, decltype(stage), StageIndex, CaseIndex>(
          stage, &sink, branch_id, input_copy, aggregate);
      const bool case_succeeded = !aggregate.template get<CaseIndex>().failed();
      sink.on_fan_in_case_completed(branch_id, CaseIndex, branch_stage_id, case_succeeded);
      return stage_result;
    });
  } else {
    return pool.enqueue([stage = std::forward<BranchStageObject>(branch_stage), &sink, branch_id, branch_stage_id,
                         &aggregate, input_ptr = &input]() mutable {
      auto stage_result = detail::run_fan_in_case_stage<BranchNode, BranchStage, decltype(stage), StageIndex, CaseIndex>(
          stage, &sink, branch_id, *input_ptr, aggregate);
      const bool case_succeeded = !aggregate.template get<CaseIndex>().failed();
      sink.on_fan_in_case_completed(branch_id, CaseIndex, branch_stage_id, case_succeeded);
      return stage_result;
    });
  }
}

template <class Futures>
[[nodiscard]] auto wait_for_fan_in_cases(Futures& futures) -> result<void> {
  for (auto& future : futures) {
    try {
      auto case_result = future.get();
      if (!case_result.has_value()) {
        return case_result;
      }
    } catch (const std::exception& exception) {
      return result<void>{error{.category = error_category::exception, .message = exception.what()}};
    } catch (...) {
      return result<void>{error{.category = error_category::exception, .message = "unknown thread-pool fan-in task failure"}};
    }
  }
  return {};
}

template <class BranchNode, std::size_t StageIndex, class Input, class Aggregate,
          std::size_t CaseIndex, class Case, class... Rest>
void schedule_fan_in_cases(thread_pool& pool, synchronized_observer& sink, const Input& input,
                           Aggregate& aggregate, std::vector<std::future<result<void>>>& futures,
                           detail::fan_in_case_tally& tally) {
  using Predicate = typename Case::predicate_type;
  using BranchStage = typename Case::stage_type;

  auto branch_id = detail::stage_id_for<BranchNode>(StageIndex);
  auto predicate_id = detail::branch_child_stage_id<BranchNode, Predicate>(StageIndex, CaseIndex, "predicate");
  auto branch_stage_id = detail::branch_child_stage_id<BranchNode, BranchStage>(StageIndex, CaseIndex, "stage");

  auto predicate = Predicate{};
  auto predicate_result = detail::evaluate_fan_in_predicate<BranchNode, Predicate, decltype(predicate), StageIndex>(
      predicate, &sink, input, CaseIndex);

  if (!predicate_result.has_value()) {
    auto predicate_error = std::move(predicate_result).error();
    detail::mark_fan_in_case_failed<Aggregate, CaseIndex>(aggregate, predicate_error);
    ++tally.failed;
    sink.on_case_failed(branch_id, CaseIndex, predicate_id, predicate_error);
  } else if (predicate_result.value()) {
    ++tally.selected;
    sink.on_case_selected(branch_id, CaseIndex, predicate_id);
    sink.on_fan_in_case_scheduled(branch_id, CaseIndex, branch_stage_id);
    auto branch_stage = BranchStage{};
    futures.push_back(enqueue_fan_in_case<BranchNode, BranchStage, decltype(branch_stage), StageIndex, CaseIndex>(
        pool, std::move(branch_stage), sink, branch_id, input, aggregate));
  } else {
    sink.on_case_skipped(branch_id, CaseIndex, predicate_id);
  }

  if constexpr (sizeof...(Rest) > 0) {
    schedule_fan_in_cases<BranchNode, StageIndex, Input, Aggregate, CaseIndex + 1, Rest...>(
        pool, sink, input, aggregate, futures, tally);
  }
}

template <class Aggregate, std::size_t... Is>
void tally_fan_in_completions(const Aggregate& aggregate, detail::fan_in_case_tally& tally,
                              std::index_sequence<Is...>) {
  // Count slots that finished their branch stage successfully.  Predicate
  // failures leave a slot in the failed state and were already tallied during
  // scheduling, so only successfully-completed selected cases match here.
  auto account = [&]<std::size_t Index>() {
    if (aggregate.template get<Index>().completed()) ++tally.completed;
  };
  (account.template operator()<Is>(), ...);
}

template <class BranchNode, std::size_t StageIndex, class Input, class... Cases>
[[nodiscard]] auto run_parallel_fan_in_branch(thread_pool& pool, observer* sink, const Input& input,
                                             pb::meta::type_list<Cases...>) -> result<typename BranchNode::output_type> {
  typename BranchNode::output_type aggregate{};
  synchronized_observer synchronized{sink};
  std::vector<std::future<result<void>>> futures;
  futures.reserve(sizeof...(Cases));

  auto branch_id = detail::stage_id_for<BranchNode>(StageIndex);
  synchronized.on_fan_in_started(branch_id, sizeof...(Cases));
  detail::fan_in_case_tally tally{};

  if constexpr (sizeof...(Cases) > 0) {
    schedule_fan_in_cases<BranchNode, StageIndex, Input, typename BranchNode::output_type, 0, Cases...>(
        pool, synchronized, input, aggregate, futures, tally);
  }

  auto wait_result = wait_for_fan_in_cases(futures);

  // Each selected case marked its slot as completed or failed in its worker
  // task.  The selected count and the predicate-failure failures were tallied
  // during scheduling; here we add the stage-level completions, then derive the
  // selected-but-failed stages as `selected - completed`.
  if constexpr (sizeof...(Cases) > 0) {
    tally_fan_in_completions(aggregate, tally, std::index_sequence_for<Cases...>{});
  }
  tally.failed += (tally.selected - tally.completed);

  synchronized.on_fan_in_completed(branch_id, tally.selected, tally.completed, tally.failed);

  if (!wait_result.has_value()) {
    return result<typename BranchNode::output_type>{std::move(wait_result).error()};
  }
  return result<typename BranchNode::output_type>{std::move(aggregate)};
}

template <class Stage, std::size_t StageIndex, class Input>
[[nodiscard]] decltype(auto) invoke_stage(thread_pool& pool, observer* sink, Input&& input) {
  if constexpr (pb::core::detail::is_fan_in_branch_node<Stage>::value) {
    return run_parallel_fan_in_branch<Stage, StageIndex>(pool, sink, input, typename Stage::cases{});
  } else if constexpr (pb::core::detail::is_selected_branch_node<Stage>::value) {
    return detail::run_selected_branch<Stage, StageIndex>(sink, std::forward<Input>(input));
  } else if constexpr (detail::stage_declares_type_list_input_v<Stage>) {
    auto stage = Stage{};
    return detail::invoke_type_list_join_stage<Stage>(stage, std::forward<Input>(input));
  } else {
    return Stage{}(std::forward<Input>(input));
  }
}

template <std::size_t StageIndex, class FinalOutput, class Input, class Stage, class... Rest>
[[nodiscard]] auto run_stages(thread_pool& pool, observer* sink, Input&& input) -> result<FinalOutput> {
  try {
    detail::notify_stage_start<Stage>(sink, StageIndex);
    auto stage_result = invoke_stage<Stage, StageIndex>(pool, sink, std::forward<Input>(input));
    auto normalized = to_result(std::move(stage_result));
    if (!normalized.has_value()) {
      auto normalized_error = detail::normalize_stage_error<Stage>(StageIndex, std::move(normalized));
      detail::notify_stage_failure<Stage>(sink, StageIndex, normalized_error);
      return result<FinalOutput>{std::move(normalized_error)};
    }
    detail::notify_stage_success<Stage>(sink, StageIndex);

    if constexpr (sizeof...(Rest) == 0) {
      if constexpr (std::is_void_v<FinalOutput>) {
        return result<FinalOutput>{};
      } else {
        return result<FinalOutput>{std::move(normalized).value()};
      }
    } else {
      return run_stages<StageIndex + 1, FinalOutput, decltype(std::move(normalized).value()), Rest...>(
          pool, sink, std::move(normalized).value());
    }
  } catch (const std::exception& exception) {
    auto stage_error = detail::exception_error<Stage>(StageIndex, exception);
    detail::notify_stage_exception<Stage>(sink, StageIndex, stage_error);
    return result<FinalOutput>{std::move(stage_error)};
  } catch (...) {
    auto stage_error = detail::unknown_exception_error<Stage>(StageIndex);
    detail::notify_stage_exception<Stage>(sink, StageIndex, stage_error);
    return result<FinalOutput>{std::move(stage_error)};
  }
}

template <class Pipeline>
class thread_pool_engine;

template <class Input, class Output, class... Stages, class Policies>
class thread_pool_engine<pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>, Policies>> {
public:
  using pipeline_type = pb::core::pipeline<Input, Output, pb::meta::type_list<Stages...>, Policies>;
  using input_type = Input;
  using output_type = Output;
  using stages = pb::meta::type_list<Stages...>;
  using try_result_type = result<Output>;
  using runtime_descriptor_type = decltype(make_descriptor<pipeline_type>());

  explicit thread_pool_engine(thread_pool_backend backend = {}) : pool_{backend.worker_count} {}

  void set_observer(observer* value) noexcept { observer_ = value; }
  [[nodiscard]] observer* get_observer() const noexcept { return observer_; }
  [[nodiscard]] std::size_t worker_count() const noexcept { return pool_.worker_count(); }
  [[nodiscard]] constexpr auto descriptor() const noexcept -> runtime_descriptor_type { return make_descriptor<pipeline_type>(); }

  [[nodiscard]] auto run(Input input) -> result<Output> { return try_run(std::move(input)); }

  [[nodiscard]] auto try_run(Input input) -> result<Output> {
    if constexpr (sizeof...(Stages) == 0) {
      return result<Output>{std::move(input)};
    } else {
      return run_stages<0, Output, Input, Stages...>(pool_, observer_, std::move(input));
    }
  }

private:
  observer* observer_{nullptr};
  thread_pool pool_;
};

} // namespace detail_thread_pool

template <pb::core::ValidPipeline Pipeline>
[[nodiscard]] auto compile(thread_pool_backend backend) {
  return detail_thread_pool::thread_pool_engine<Pipeline>{backend};
}

} // namespace pb::runtime

namespace pb {
using runtime::compile;
using runtime::thread_pool_backend;
}
