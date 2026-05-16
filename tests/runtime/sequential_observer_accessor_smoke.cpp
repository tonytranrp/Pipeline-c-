#include <pb/pipeline.hpp>

#include <cassert>

struct Input {
  int value{};
};

struct Output {
  int value{};
};

struct AddOne {
  using input_type = Input;
  using output_type = Output;

  static constexpr auto stage_name() noexcept { return "add_one"; }

  Output operator()(Input input) const { return {input.value + 1}; }
};

struct CheckedDouble {
  using input_type = Output;
  using output_type = Output;

  static constexpr auto stage_name() noexcept { return "checked_double"; }

  pb::runtime::result<Output> operator()(Output input) const {
    if (input.value < 0) {
      return pb::runtime::error{.category = pb::runtime::error_category::stage_failure, .message = "negative input"};
    }
    return Output{input.value * 2};
  }
};

struct NullObserver : pb::runtime::observer {};

struct CountingObserver final : pb::runtime::observer {
  int starts{};
  int successes{};
  int failures{};
  int exceptions{};

  void on_stage_start(const pb::runtime::stage_id&) override { ++starts; }
  void on_stage_success(const pb::runtime::stage_id&) override { ++successes; }
  void on_stage_failure(const pb::runtime::stage_id&, const pb::runtime::error&) override { ++failures; }
  void on_stage_exception(const pb::runtime::stage_id&, const pb::runtime::error&) override { ++exceptions; }
};

using Pipeline = pb::from<Input>::then<AddOne>::to<Output>;
using FailingPipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  assert(engine.get_observer() == nullptr);

  const auto descriptor = engine.describe();
  static_assert(decltype(engine)::stage_count == 1);
  assert(descriptor.size() == 1);
  assert(descriptor[0].key == "add_one");
  assert(descriptor[0].name == "add_one");

  NullObserver observer{};
  engine.set_observer(&observer);
  assert(engine.get_observer() == &observer);

  auto ok = engine.try_run(Input{10});
  assert(ok.has_value());
  assert(ok.value().value == 11);

  engine.set_observer(nullptr);
  assert(engine.get_observer() == nullptr);

  auto output = engine.run(Input{20});
  if (output.value != 21) {
    return 1;
  }

  CountingObserver first_observer{};
  CountingObserver second_observer{};
  auto failing_engine = pb::compile<FailingPipeline>(pb::runtime::sequential{});
  const auto failing_descriptor = failing_engine.describe();
  assert(failing_descriptor.size() == 2);
  assert(failing_descriptor[0].key == "add_one");
  assert(failing_descriptor[1].key == "checked_double");

  failing_engine.set_observer(&first_observer);
  auto first_pass = failing_engine.try_run(Input{-2});
  assert(!first_pass.has_value());
  assert(first_observer.starts > 0);
  assert(first_observer.failures > 0);
  const auto first_starts = first_observer.starts;

  failing_engine.set_observer(&second_observer);
  auto second_pass = failing_engine.try_run(Input{15});
  assert(second_pass.has_value());
  assert(second_pass.value().value == 32);
  assert(second_observer.starts > 0);
  assert(first_observer.starts == first_starts);
  assert(first_observer.exceptions == 0);
  assert(first_observer.failures >= 1);
  const auto second_starts = second_observer.starts;

  failing_engine.set_observer(nullptr);
  auto silent_run = failing_engine.try_run(Input{15});
  assert(silent_run.has_value());
  assert(silent_run.value().value == 32);
  assert(first_observer.starts == first_starts);
  assert(second_observer.starts == second_starts);

  return 0;
}
