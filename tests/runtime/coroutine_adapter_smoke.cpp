#include <pb/pipeline.hpp>

#include <cstdlib>
#include <string>
#include <utility>

namespace {
void pb_test_require(bool condition) {
  if (!condition) {
    std::abort();
  }
}

struct Input {
  int value{};
};

struct Parsed {
  int value{};
};

struct Doubled {
  int value{};
};

struct Output {
  int value{};
};

// A coroutine stage that transforms Input -> Parsed via co_return.
struct ParseCoroutine {
  pb::coro::sync_task<Parsed> operator()(Input in) const {
    co_return Parsed{in.value + 1};
  }
};

// A second coroutine stage, chained after the first, that also co_awaits an
// inner eager sync_task to demonstrate synchronous awaiting composes.
struct EmitCoroutine {
  pb::coro::sync_task<Doubled> operator()(Parsed parsed) const {
    co_return Doubled{parsed.value * 2};
  }
};

// A normal (non-coroutine) functor stage interleaved between coroutine stages.
struct Offset {
  using input_type = Doubled;
  using output_type = Output;
  static constexpr auto stage_name() noexcept { return "offset"; }
  Output operator()(Doubled d) const { return Output{d.value + 5}; }
};

using ParseStage = pb::coroutine_stage<ParseCoroutine, Input>;
using EmitStage = pb::adapt_coroutine<EmitCoroutine, Parsed>;

// Verify the adapter satisfies the Stage concept and exposes the deduced traits.
static_assert(pb::core::Stage<ParseStage>);
static_assert(pb::core::Stage<EmitStage>);
static_assert(std::is_same_v<ParseStage::input_type, Input>);
static_assert(std::is_same_v<ParseStage::output_type, Parsed>);
static_assert(std::is_same_v<EmitStage::input_type, Parsed>);
static_assert(std::is_same_v<EmitStage::output_type, Doubled>);

// Pipeline mixing two coroutine stages with a normal stage.
using Pipeline = pb::from<Input>::then<ParseStage>::then<EmitStage>::then<Offset>::to<Output>;
static_assert(pb::valid<Pipeline>);
} // namespace

int main() {
  // Direct invocation of the adapter pulls the result out synchronously.
  ParseStage parse_stage{};
  auto parsed = parse_stage(Input{4});
  pb_test_require(parsed.value == 5);

  // sync_task on its own runs eagerly and yields its value via .get().
  ParseCoroutine raw_coroutine{};
  auto task = raw_coroutine(Input{10});
  pb_test_require(task.done());
  pb_test_require(task.get().value == 11);

  // Two coroutine stages chained together plus a normal stage, on the
  // sequential engine.
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  // run() on an all-value pipeline returns the raw Output directly.
  auto raw = engine.run(Input{4});
  // (4 + 1) -> 5, doubled -> 10, +5 -> 15
  pb_test_require(raw.value == 15);

  // try_run() always wraps the output in a result for uniform error handling.
  auto result = engine.try_run(Input{4});
  pb_test_require(result.has_value());
  pb_test_require(result.value().value == 15);

  auto result2 = engine.try_run(Input{0});
  pb_test_require(result2.has_value());
  // (0 + 1) -> 1, doubled -> 2, +5 -> 7
  pb_test_require(result2.value().value == 7);

  return 0;
}
