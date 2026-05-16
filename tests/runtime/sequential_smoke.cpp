#include <pb/pipeline.hpp>

#include <cassert>
#include <memory>
#include <string_view>
#include <type_traits>

struct Input { int value{}; };
struct Middle { int value{}; };
struct Output { int value{}; };

struct AddOne {
  using input_type = Input;
  using output_type = Middle;
  static constexpr auto key = pb::fixed_string{"runtime.add_one"};
  static constexpr auto name = pb::fixed_string{"add_one"};
  Middle operator()(Input input) const { return {input.value + 1}; }
};

struct DoubleValue {
  using input_type = Middle;
  using output_type = Output;
  static constexpr auto key = pb::fixed_string{"runtime.double"};
  static constexpr auto name = pb::fixed_string{"double"};
  Output operator()(Middle input) const { return {input.value * 2}; }
};

using Pipeline = pb::from<Input>::then<AddOne>::then<DoubleValue>::to<Output>;

struct MoveInput {
  std::unique_ptr<int> value{};
};

struct MoveMiddle {
  std::unique_ptr<int> value{};
};

struct MoveOutput {
  std::unique_ptr<int> value{};
};

static_assert(!std::copy_constructible<MoveInput>);
static_assert(!std::copy_constructible<MoveMiddle>);
static_assert(!std::copy_constructible<MoveOutput>);

struct MoveAddOne {
  using input_type = MoveInput;
  using output_type = MoveMiddle;

  MoveMiddle operator()(MoveInput input) const {
    return {std::make_unique<int>(*input.value + 1)};
  }
};

struct MoveDoubleValue {
  using input_type = MoveMiddle;
  using output_type = MoveOutput;

  MoveOutput operator()(MoveMiddle input) const {
    return {std::make_unique<int>(*input.value * 2)};
  }
};

using MoveOnlyPipeline = pb::from<MoveInput>::then<MoveAddOne>::then<MoveDoubleValue>::to<MoveOutput>;
static_assert(pb::valid<MoveOnlyPipeline>);

struct StatefulInput { int value{}; };
struct StatefulOutput { int value{}; };

struct CountingStage {
  using input_type = StatefulInput;
  using output_type = StatefulOutput;

  int calls{};

  StatefulOutput operator()(StatefulInput input) {
    ++calls;
    return {input.value + calls};
  }
};

using StatefulPipeline = pb::from<StatefulInput>::then<CountingStage>::to<StatefulOutput>;
static_assert(pb::valid<StatefulPipeline>);

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  using Engine = decltype(engine);
  static_assert(std::is_same_v<typename Engine::pipeline_type, Pipeline>);
  static_assert(std::is_same_v<typename Engine::input_type, Input>);
  static_assert(std::is_same_v<typename Engine::output_type, Output>);
  static_assert(std::is_same_v<typename Engine::stage_storage_policy, pb::runtime::construct_stages_per_run>);
  static_assert(std::is_same_v<typename Engine::try_result_type, pb::runtime::result<Output>>);
  static_assert(Engine::stage_count == 2);

  constexpr auto descriptor_view = pb::descriptor_view<Pipeline>();
  static_assert(decltype(descriptor_view)::stage_count == 2);
  static_assert(!decltype(descriptor_view)::empty);
  static_assert(descriptor_view.stage_records()[0].index == 0);
  static_assert(descriptor_view.stage_records()[0].key == std::string_view{"runtime.add_one"});
  static_assert(descriptor_view.stage_records()[0].name == std::string_view{"add_one"});
  static_assert(descriptor_view.stage_records()[1].index == 1);
  static_assert(descriptor_view.stage_records()[1].key == std::string_view{"runtime.double"});
  static_assert(descriptor_view.stage_records()[1].name == std::string_view{"double"});

  auto output = engine.run(Input{20});
  if (output.value != 42) {
    return 1;
  }

  auto move_engine = pb::compile<MoveOnlyPipeline>(pb::runtime::sequential{});
  auto move_output = move_engine.run(MoveInput{std::make_unique<int>(20)});
  if (*move_output.value != 42) {
    return 1;
  }

  auto safe_move_output = move_engine.try_run(MoveInput{std::make_unique<int>(21)});
  assert(safe_move_output.has_value());
  assert(*safe_move_output.value().value == 44);

  auto per_run_engine = pb::compile<StatefulPipeline>(pb::runtime::sequential{});
  const auto per_run_first = per_run_engine.run(StatefulInput{10});
  const auto per_run_second = per_run_engine.run(StatefulInput{10});
  if (per_run_first.value != 11 || per_run_second.value != 11) {
    return 1;
  }

  auto stateful_engine = pb::compile<StatefulPipeline>(pb::runtime::stateful_sequential{});
  using StatefulEngine = decltype(stateful_engine);
  static_assert(std::is_same_v<typename StatefulEngine::stage_storage_policy, pb::runtime::store_stages_in_engine>);
  const auto stateful_first = stateful_engine.run(StatefulInput{10});
  const auto stateful_second = stateful_engine.run(StatefulInput{10});
  auto stateful_third = stateful_engine.try_run(StatefulInput{10});
  assert(stateful_third.has_value());
  if (stateful_first.value != 11 || stateful_second.value != 12 || stateful_third.value().value != 13) {
    return 1;
  }

  return 0;
}
