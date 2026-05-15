#include <pb/pipeline.hpp>

#include <cassert>
#include <stdexcept>
#include <string>

struct Input {
  int value{};
};

struct Middle {
  int value{};
};

struct Output {
  int value{};
};

struct AddOne {
  using input_type = Input;
  using output_type = Middle;

  Middle operator()(Input input) const { return {input.value + 1}; }
};

struct MaybeThrowingDouble {
  using input_type = Middle;
  using output_type = Output;

  Output operator()(Middle input) const {
    if (input.value < 0) {
      throw std::runtime_error{"negative middle"};
    }
    return {input.value * 2};
  }
};

struct CheckedDouble {
  using input_type = Middle;
  using output_type = Output;

  pb::runtime::result<Output> operator()(Middle input) const {
    if (input.value == 0) {
      return pb::runtime::error{.category = pb::runtime::error_category::stage_failure,
                                .message = "zero middle"};
    }
    return Output{input.value * 2};
  }
};

using ThrowingPipeline = pb::from<Input>::then<AddOne>::then<MaybeThrowingDouble>::to<Output>;
using ResultPipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
static_assert(pb::valid<ThrowingPipeline>);
static_assert(pb::valid<ResultPipeline>);

int main() {
  auto throwing_engine = pb::compile<ThrowingPipeline>(pb::runtime::sequential{});

  auto ok = throwing_engine.try_run(Input{20});
  assert(ok.has_value());
  assert(ok.value().value == 42);

  auto exception = throwing_engine.try_run(Input{-2});
  assert(!exception.has_value());
  assert(exception.error().category == pb::runtime::error_category::exception);
  assert(exception.error().message == "negative middle");

  auto result_engine = pb::compile<ResultPipeline>(pb::runtime::sequential{});
  auto failed = result_engine.try_run(Input{-1});
  assert(!failed.has_value());
  assert(failed.error().category == pb::runtime::error_category::stage_failure);
  assert(failed.error().message == "zero middle");

  return 0;
}
