#include <pb/pipeline.hpp>

#include <cassert>
#include <type_traits>

struct Input {
  int value{};
};

struct Output {
  int value{};
};

struct CountingStage {
  using input_type = Input;
  using output_type = Output;

  int calls{};

  Output operator()(Input input) {
    ++calls;
    return {input.value + calls};
  }
};

using Pipeline = pb::from<Input>::then<CountingStage>::to<Output>;

int main() {
  auto per_run = pb::compile<Pipeline>(pb::policy::sequential<pb::policy::storage::construct_per_run>{});
  using PerRunEngine = decltype(per_run);
  static_assert(std::is_same_v<typename PerRunEngine::stage_storage_policy, pb::runtime::construct_stages_per_run>);

  const auto per_run_first = per_run.run(Input{10});
  const auto per_run_second = per_run.try_run(Input{10});
  assert(per_run_second.has_value());
  assert(per_run_first.value == 11);
  assert(per_run_second.value().value == 11);

  auto stateful = pb::compile<Pipeline>(pb::policy::sequential<pb::policy::storage::store_in_engine>{});
  using StatefulEngine = decltype(stateful);
  static_assert(std::is_same_v<typename StatefulEngine::stage_storage_policy, pb::runtime::store_stages_in_engine>);

  const auto stateful_first = stateful.run(Input{10});
  const auto stateful_second = stateful.try_run(Input{10});
  const auto stateful_third = stateful.run(Input{10});
  assert(stateful_second.has_value());
  assert(stateful_first.value == 11);
  assert(stateful_second.value().value == 12);
  assert(stateful_third.value == 13);
}
