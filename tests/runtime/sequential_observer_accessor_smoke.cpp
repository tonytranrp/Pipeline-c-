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

struct NullObserver : pb::runtime::observer {};

using Pipeline = pb::from<Input>::then<AddOne>::to<Output>;

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  assert(engine.get_observer() == nullptr);

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

  return 0;
}
