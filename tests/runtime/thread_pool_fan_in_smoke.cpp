#include <pb/pipeline.hpp>

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace {

void require_impl(bool condition, int line) {
  if (!condition) {
    std::cerr << "require failed at line " << line << '\n';
    std::abort();
  }
}

#define require(condition) require_impl((condition), __LINE__)

struct Input { int mask{}; int value{}; };
struct Output { int value{}; };
struct Done { std::string text{}; };
struct VoidDone { int completed{}; int skipped{}; int failed{}; };

struct IsA {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 1) != 0; }
};
struct IsB {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 2) != 0; }
};
struct IsFailing {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 4) != 0; }
};

struct ParallelProbe {
  static inline std::atomic<int> active{0};
  static inline std::atomic<int> entered{0};
  static inline std::atomic<int> max_active{0};

  static void reset() {
    active = 0;
    entered = 0;
    max_active = 0;
  }

  static void wait_for_overlap() {
    const auto now_active = ++active;
    ++entered;
    int observed = max_active.load();
    while (observed < now_active && !max_active.compare_exchange_weak(observed, now_active)) {}
    while (entered.load() < 2) {
      std::this_thread::yield();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds{10});
    --active;
  }
};

struct SlowA {
  using input_type = Input;
  using output_type = Output;
  Output operator()(Input input) const {
    ParallelProbe::wait_for_overlap();
    return Output{input.value + 10};
  }
};
struct SlowB {
  using input_type = Input;
  using output_type = Output;
  Output operator()(Input input) const {
    ParallelProbe::wait_for_overlap();
    return Output{input.value + 100};
  }
};
struct FailingStage {
  using input_type = Input;
  using output_type = Output;
  pb::runtime::result<Output> operator()(Input) const {
    return pb::runtime::result<Output>{pb::runtime::error{.message = "thread-pool stage failure"}};
  }
};

struct FastBeforeObserverFailureStage {
  using input_type = Input;
  using output_type = Output;
  Output operator()(Input input) const { return Output{input.value}; }
};

struct SlowAfterObserverFailureStage {
  using input_type = Input;
  using output_type = Output;
  static inline std::atomic<int> completed{0};
  Output operator()(Input input) const {
    std::this_thread::sleep_for(std::chrono::milliseconds{50});
    ++completed;
    return Output{input.value + 1000};
  }
};

using CaseA = pb::case_<IsA>::then<SlowA>;
using CaseB = pb::case_<IsB>::then<SlowB>;
using FailCase = pb::case_<IsFailing>::then<FailingStage>;
using FanInInput = pb::fan_in_output_t<CaseA, CaseB, FailCase>;

struct Join {
  using input_type = FanInInput;
  using output_type = Done;
  Done operator()(const FanInInput& input) const {
    std::string text;
    text += input.template get<0>().completed() ? "A=" + std::to_string(input.template get<0>().get().value) : "A=skip";
    text += ";";
    text += input.template get<1>().completed() ? "B=" + std::to_string(input.template get<1>().get().value) : "B=skip";
    text += ";";
    text += input.template get<2>().failed() ? "F=" + std::string{input.template get<2>().diagnostic_message()} : "F=ok";
    return Done{std::move(text)};
  }
};
using Pipeline = pb::from<Input>::branch<CaseA, CaseB, FailCase>::fan_in<Join>::to<Done>;
static_assert(pb::valid<Pipeline>);

using FastBeforeObserverFailureCase = pb::case_<IsA>::then<FastBeforeObserverFailureStage>;
using SlowAfterObserverFailureCase = pb::case_<IsB>::then<SlowAfterObserverFailureStage>;
using DrainInput = pb::fan_in_output_t<FastBeforeObserverFailureCase, SlowAfterObserverFailureCase>;
struct DrainJoin {
  using input_type = DrainInput;
  using output_type = Done;
  Done operator()(const DrainInput&) const { return Done{"unexpected"}; }
};
using DrainPipeline =
    pb::from<Input>::branch<FastBeforeObserverFailureCase, SlowAfterObserverFailureCase>::fan_in<DrainJoin>::to<Done>;
static_assert(pb::valid<DrainPipeline>);

struct VoidStage {
  using input_type = Input;
  using output_type = void;
  static inline std::atomic<int> calls{0};
  void operator()(Input) const { ++calls; }
};
using VoidCase = pb::case_<IsA>::then<VoidStage>;
using VoidInput = pb::fan_in_output_t<VoidCase, CaseB>;
struct VoidJoin {
  using input_type = VoidInput;
  using output_type = VoidDone;
  VoidDone operator()(const VoidInput& input) const {
    return VoidDone{.completed = input.template get<0>().completed() ? 1 : 0,
                    .skipped = input.template get<1>().skipped() ? 1 : 0,
                    .failed = input.template get<0>().failed() || input.template get<1>().failed() ? 1 : 0};
  }
};
using VoidPipeline = pb::from<Input>::branch<VoidCase, CaseB>::fan_in<VoidJoin>::to<VoidDone>;

struct MoveOnlyInput {
  MoveOnlyInput(int mask_value, int payload_value) : mask(mask_value), payload(std::make_unique<int>(payload_value)) {}
  MoveOnlyInput(MoveOnlyInput&&) noexcept = default;
  MoveOnlyInput& operator=(MoveOnlyInput&&) noexcept = default;
  MoveOnlyInput(const MoveOnlyInput&) = delete;
  MoveOnlyInput& operator=(const MoveOnlyInput&) = delete;
  int mask{};
  std::unique_ptr<int> payload{};
};
struct IsMoveA {
  using input_type = MoveOnlyInput;
  using output_type = bool;
  bool operator()(const MoveOnlyInput& input) const { return (input.mask & 1) != 0; }
};
struct IsMoveB {
  using input_type = MoveOnlyInput;
  using output_type = bool;
  bool operator()(const MoveOnlyInput& input) const { return (input.mask & 2) != 0; }
};
struct BorrowA {
  using input_type = MoveOnlyInput;
  using output_type = Output;
  Output operator()(const MoveOnlyInput& input) const { return Output{*input.payload + 1}; }
};
struct BorrowB {
  using input_type = MoveOnlyInput;
  using output_type = Output;
  Output operator()(const MoveOnlyInput& input) const { return Output{*input.payload + 2}; }
};
using BorrowCaseA = pb::case_<IsMoveA>::then<BorrowA>;
using BorrowCaseB = pb::case_<IsMoveB>::then<BorrowB>;
using BorrowInput = pb::fan_in_output_t<BorrowCaseA, BorrowCaseB>;
struct BorrowJoin {
  using input_type = BorrowInput;
  using output_type = Done;
  Done operator()(const BorrowInput& input) const {
    return Done{std::to_string(input.template get<0>().get().value) + "," +
                std::to_string(input.template get<1>().get().value)};
  }
};
using BorrowPipeline = pb::from<MoveOnlyInput>::branch<BorrowCaseA, BorrowCaseB>::fan_in<BorrowJoin>::to<Done>;

struct RecordingObserver : pb::runtime::observer {
  std::vector<std::string> events;
  void on_case_selected(const pb::runtime::stage_id&, std::size_t case_index, const pb::runtime::stage_id&) override {
    events.push_back("selected:" + std::to_string(case_index));
  }
  void on_case_failed(const pb::runtime::stage_id&, std::size_t case_index, const pb::runtime::stage_id&, const pb::runtime::error& error) override {
    events.push_back("failed:" + std::to_string(case_index) + ":" + error.message);
  }
};

struct ThrowOnFirstCompletionObserver : pb::runtime::observer {
  void on_fan_in_case_completed(const pb::runtime::stage_id&, std::size_t case_index, const pb::runtime::stage_id&,
                                bool) override {
    if (case_index == 0) {
      throw std::runtime_error{"observer completion failed"};
    }
  }
};

bool contains(const std::vector<std::string>& events, std::string_view expected) {
  for (const auto& event : events) {
    if (event == expected) return true;
  }
  return false;
}

} // namespace

int main() {
  ParallelProbe::reset();
  RecordingObserver observer;
  auto engine = pb::compile<Pipeline>(pb::runtime::thread_pool_backend{.worker_count = 2});
  engine.set_observer(&observer);
  require(engine.worker_count() == 2);
  auto result = engine.run(Input{7, 5});
  require(result.has_value());
  require(result.value().text == "A=15;B=105;F=thread-pool stage failure");
  require(ParallelProbe::max_active.load() >= 2);
  require(contains(observer.events, "selected:0"));
  require(contains(observer.events, "selected:1"));
  require(contains(observer.events, "failed:2:thread-pool stage failure"));

  SlowAfterObserverFailureStage::completed = 0;
  auto drain_engine = pb::compile<DrainPipeline>(pb::runtime::thread_pool_backend{.worker_count = 2});
  ThrowOnFirstCompletionObserver throwing_observer;
  drain_engine.set_observer(&throwing_observer);
  auto drained = drain_engine.try_run(Input{3, 5});
  require(!drained.has_value());
  require(drained.error().category == pb::runtime::error_category::exception);
  require(drained.error().message == "observer completion failed");
  require(SlowAfterObserverFailureStage::completed.load() == 1);

  VoidStage::calls = 0;
  auto void_engine = pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.worker_count = 2});
  auto void_result = void_engine.try_run(Input{1, 9});
  require(void_result.has_value());
  require(void_result.value().completed == 1);
  require(void_result.value().skipped == 1);
  require(void_result.value().failed == 0);
  require(VoidStage::calls == 1);

  auto shared_pool = std::make_shared<pb::thread_pool>(3);
  auto shared_a = pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.worker_count = 99, .pool = shared_pool});
  auto shared_b = pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool});
  require(shared_a.worker_count() == 3);
  require(shared_b.worker_count() == 3);
  require(shared_a.shared_pool() == shared_pool);
  require(shared_b.shared_pool() == shared_pool);
  auto shared_result = shared_a.try_run(Input{1, 4});
  require(shared_result.has_value());
  shared_a.wait_idle();
  require(shared_a.snapshot().idle());
  require(shared_a.wait_idle_for(std::chrono::seconds{1}));
  require(shared_b.wait_idle_until(std::chrono::steady_clock::now() + std::chrono::seconds{1}));

  auto throwing = pb::with_throw_on_error(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}));
  require(throwing.worker_count() == 3);
  require(throwing.shared_pool() == shared_pool);
  require(throwing.snapshot().idle());
  auto throwing_result = throwing.try_run(Input{1, 6});
  require(throwing_result.has_value());
  throwing.wait_idle();
  require(throwing.wait_idle_for(std::chrono::seconds{1}));

  auto terminating = pb::with_terminate_on_error(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}));
  require(terminating.worker_count() == 3);
  require(terminating.snapshot().idle());
  require(terminating.wait_idle_until(std::chrono::steady_clock::now() + std::chrono::seconds{1}));

  auto propagating = pb::with_propagate_exceptions(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}));
  require(propagating.pending_tasks() == 0);
  require(propagating.active_tasks() == 0);
  require(propagating.wait_idle_for(std::chrono::seconds{1}));

  auto ignoring = pb::with_ignore_errors(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}), VoidDone{});
  require(ignoring.queued_tasks() == 0);
  require(ignoring.wait_idle_for(std::chrono::seconds{1}));

  std::ostringstream verbose_sink;
  auto verbose = pb::with_verbose_diagnostics(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}), &verbose_sink);
  require(verbose.worker_count() == 3);
  require(verbose.snapshot().idle());
  verbose.wait_idle();
  require(verbose.wait_idle_for(std::chrono::seconds{1}));

  auto stateful = pb::with_state<int>(
      pb::compile<VoidPipeline>(pb::runtime::thread_pool_backend{.pool = shared_pool}));
  require(stateful.shared_pool() == shared_pool);
  stateful.wait_idle();
  require(stateful.wait_idle_for(std::chrono::seconds{1}));

  auto borrowed = pb::compile<BorrowPipeline>(pb::runtime::thread_pool_backend{.worker_count = 2});
  auto borrowed_result = borrowed.run(MoveOnlyInput{3, 40});
  require(borrowed_result.has_value());
  require(borrowed_result.value().text == "41,42");
}
