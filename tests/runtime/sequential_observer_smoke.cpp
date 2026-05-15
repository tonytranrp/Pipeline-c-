#include <pb/pipeline.hpp>

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

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

  static constexpr auto stage_name() noexcept { return "add_one"; }

  Middle operator()(Input input) const { return {input.value + 1}; }
};

struct CheckedDouble {
  using input_type = Middle;
  using output_type = Output;

  static constexpr auto stage_name() noexcept { return "checked_double"; }
  static constexpr auto stage_key() noexcept { return "math.double"; }

  pb::runtime::result<Output> operator()(Middle input) const {
    if (input.value == 0) {
      return pb::runtime::error{.category = pb::runtime::error_category::stage_failure,
                                .message = "zero middle"};
    }
    return Output{input.value * 2};
  }
};

struct ThrowingStage {
  using input_type = Middle;
  using output_type = Output;

  static constexpr auto stage_name() noexcept { return "throwing_stage"; }

  Output operator()(Middle) const { throw std::runtime_error{"boom"}; }
};

using SuccessPipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
using FailurePipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
using ThrowPipeline = pb::from<Input>::then<AddOne>::then<ThrowingStage>::to<Output>;

struct recording_observer final : pb::runtime::observer {
  std::vector<std::string> events{};

  void on_stage_start(const pb::runtime::stage_id& stage) override {
    events.push_back("start:" + stage.name + "/" + stage.key);
  }

  void on_stage_success(const pb::runtime::stage_id& stage) override {
    events.push_back("success:" + stage.name + "/" + stage.key);
  }

  void on_stage_failure(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("failure:" + stage.name + "/" + stage.key + ":" + error.message);
  }

  void on_stage_exception(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("exception:" + stage.name + "/" + stage.key + ":" + error.message);
  }
};

int main() {
  {
    auto engine = pb::compile<SuccessPipeline>(pb::runtime::sequential{});
    recording_observer observer{};
    engine.set_observer(&observer);

    auto output = engine.run(Input{20});
    assert(output.has_value());
    assert(output.value().value == 42);
    assert((observer.events == std::vector<std::string>{
                "start:add_one/add_one",
                "success:add_one/add_one",
                "start:checked_double/math.double",
                "success:checked_double/math.double",
            }));
  }

  {
    auto engine = pb::compile<FailurePipeline>(pb::runtime::sequential{});
    recording_observer observer{};
    engine.set_observer(&observer);

    auto failed = engine.try_run(Input{-1});
    assert(!failed.has_value());
    assert(failed.error().category == pb::runtime::error_category::stage_failure);
    assert((observer.events == std::vector<std::string>{
                "start:add_one/add_one",
                "success:add_one/add_one",
                "start:checked_double/math.double",
                "failure:checked_double/math.double:zero middle",
            }));
  }

  {
    auto engine = pb::compile<ThrowPipeline>(pb::runtime::sequential{});
    recording_observer observer{};
    engine.set_observer(&observer);

    try {
      (void)engine.run(Input{1});
      return 1;
    } catch (const std::runtime_error& error) {
      if (std::string_view{error.what()} != "boom") {
        return 1;
      }
    }
    assert((observer.events == std::vector<std::string>{
                "start:add_one/add_one",
                "success:add_one/add_one",
                "start:throwing_stage/throwing_stage",
                "exception:throwing_stage/throwing_stage:boom",
            }));
  }

  return 0;
}
