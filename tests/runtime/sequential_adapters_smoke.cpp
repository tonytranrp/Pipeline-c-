#include <pb/pipeline.hpp>

#include <cassert>
#include <exception>
#include <string>
#include <string_view>
#include <vector>

struct Input {
  int value{};
};

struct Parsed {
  int value{};
};

struct Output {
  int value{};
};

namespace adapter_stage_names {
struct parse_input {
  static constexpr auto value = "parse_input";
};

struct multiply_input {
  static constexpr auto value = "multiply_input";
};
} // namespace adapter_stage_names

Parsed parse_input_fn(Input input) {
  return Parsed{input.value + 1};
}

Parsed parse_input_throwing(Input input) {
  if (input.value < -100) {
    throw std::runtime_error{"parse_input_throwing failed"};
  }
  return Parsed{input.value + 2};
}

Parsed double_input(Parsed parsed) {
  return Parsed{parsed.value * 2};
}

using ParsedAdapter = pb::adapt<pb::name<adapter_stage_names::parse_input>, pb::fn<parse_input_fn>,
                                 pb::in<Input>, pb::out<Parsed>>;
using ThrowingParsedAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_input>, pb::fn<parse_input_throwing>, pb::in<Input>,
             pb::out<Parsed>>;
using MultiplierAdapter =
    pb::adapt<pb::name<adapter_stage_names::multiply_input>, pb::fn<double_input>,
             pb::in<Parsed>, pb::out<Parsed>>;

struct Emit {
  using input_type = Parsed;
  using output_type = Output;
  using error_type = pb::runtime::error;

  static constexpr auto stage_key() noexcept { return "emit"; }
  static constexpr auto stage_name() noexcept { return "emit_result"; }

  pb::runtime::result<Output> operator()(Parsed parsed) const {
    if (parsed.value < 0) {
      return pb::runtime::error{.stage = {.key = "emit", .name = "emit_result"},
                               .category = pb::runtime::error_category::stage_failure,
                               .message = "invalid parsed value"};
    }
    return Output{parsed.value + 1};
  }
};

using Pipeline = pb::from<Input>::then<ParsedAdapter>::then<MultiplierAdapter>::then<Emit>::to<Output>;
using ThrowingPipeline =
    pb::from<Input>::then<ThrowingParsedAdapter>::then<MultiplierAdapter>::then<Emit>::to<Output>;
using ThrowingPipelineRaw = pb::from<Input>::then<ThrowingParsedAdapter>::then<MultiplierAdapter>::to<Parsed>;

static_assert(pb::adapted_stage<ParsedAdapter>);
static_assert(pb::adapted_stage<MultiplierAdapter>);
static_assert(pb::valid<Pipeline>);
static_assert(pb::valid<ThrowingPipeline>);
static_assert(pb::valid<ThrowingPipelineRaw>);

struct recording_observer final : pb::runtime::observer {
  std::vector<std::string> events{};

  void on_stage_start(const pb::runtime::stage_id& stage) override {
    events.push_back("start:" + stage.name + "/" + stage.key);
  }

  void on_stage_exception(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("exception:" + stage.name + "/" + stage.key + ":" + error.message);
  }

  void on_stage_success(const pb::runtime::stage_id& stage) override {
    events.push_back("success:" + stage.name + "/" + stage.key);
  }

  void on_stage_failure(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("failure:" + stage.name + "/" + stage.key + ":" + error.message);
  }
};

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto ok = engine.run(Input{4});
  assert(ok.has_value());
  assert(ok.value().value == 11);

  auto failed = engine.run(Input{-2});
  assert(!failed.has_value());
  assert(failed.error().stage.key == "emit");
  assert(failed.error().stage.name == "emit_result");
  assert(failed.error().category == pb::runtime::error_category::stage_failure);
  assert(failed.error().message == "invalid parsed value");

  auto throwing_result_engine = pb::compile<ThrowingPipeline>(pb::runtime::sequential{});
  recording_observer observer{};
  throwing_result_engine.set_observer(&observer);

  auto result_caught = throwing_result_engine.try_run(Input{-200});
  assert(!result_caught.has_value());
  assert(result_caught.error().category == pb::runtime::error_category::exception);
  assert(result_caught.error().stage.name == "parse_input");
  assert(result_caught.error().stage.key == "parse_input");
  assert(result_caught.error().message == "parse_input_throwing failed");

  auto throwing_raw_engine = pb::compile<ThrowingPipelineRaw>(pb::runtime::sequential{});
  throwing_raw_engine.set_observer(&observer);

  try {
    (void)throwing_raw_engine.run(Input{-200});
    return 1;
  } catch (const std::runtime_error& error) {
    assert(std::string_view{error.what()} == "parse_input_throwing failed");
  }

  assert((observer.events == std::vector<std::string>{
              "start:parse_input/parse_input",
              "exception:parse_input/parse_input:parse_input_throwing failed",
              "start:parse_input/parse_input",
              "exception:parse_input/parse_input:parse_input_throwing failed",
          }));

  return 0;
}
