#include <pb/pipeline.hpp>

#include <cassert>
#include <string>

struct Input { int value{}; };
struct Middle { int value{}; };
struct Output { int value{}; };

struct AddOne {
  using input_type = Input;
  using output_type = Middle;
  Middle operator()(Input input) const { return {input.value + 1}; }
};

struct CheckedDouble {
  using input_type = Middle;
  using output_type = Output;
  using error_type = pb::runtime::error;

  pb::runtime::result<Output> operator()(Middle input) const {
    if (input.value < 0) {
      return pb::runtime::error{.stage = {.key = "checked-double", .name = "checked_double"},
                                .category = pb::runtime::error_category::stage_failure,
                                .message = "negative input"};
    }
    return Output{input.value * 2};
  }
};

using Pipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
static_assert(pb::valid<Pipeline>);

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto ok = engine.run(Input{20});
  assert(ok.has_value());
  assert(ok.value().value == 42);

  auto failed = engine.run(Input{-2});
  assert(!failed.has_value());
  assert(failed.error().stage.name == "checked_double");
  assert(failed.error().message == "negative input");

  return 0;
}
