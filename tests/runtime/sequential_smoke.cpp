#include <pb/pipeline.hpp>

#include <type_traits>

struct Input { int value{}; };
struct Middle { int value{}; };
struct Output { int value{}; };

struct AddOne {
  using input_type = Input;
  using output_type = Middle;
  Middle operator()(Input input) const { return {input.value + 1}; }
};

struct DoubleValue {
  using input_type = Middle;
  using output_type = Output;
  Output operator()(Middle input) const { return {input.value * 2}; }
};

using Pipeline = pb::from<Input>::then<AddOne>::then<DoubleValue>::to<Output>;

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  using Engine = decltype(engine);
  static_assert(std::is_same_v<typename Engine::pipeline_type, Pipeline>);
  static_assert(std::is_same_v<typename Engine::input_type, Input>);
  static_assert(std::is_same_v<typename Engine::output_type, Output>);
  static_assert(Engine::stage_count == 2);

  auto output = engine.run(Input{20});
  return output.value == 42 ? 0 : 1;
}
