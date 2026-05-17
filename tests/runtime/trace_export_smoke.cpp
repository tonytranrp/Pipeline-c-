#include <pb/pipeline.hpp>

#include <cassert>
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
                                .message = "zero \"middle\"\nline"};
    }
    return Output{input.value * 2};
  }
};

using Pipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
static_assert(pb::valid<Pipeline>);

[[nodiscard]] auto contains(std::string_view haystack, std::string_view needle) -> bool {
  return haystack.find(needle) != std::string_view::npos;
}

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  pb::runtime::trace_recorder recorder{};
  pb::runtime::trace_observer observer{recorder};
  engine.set_observer(&observer);

  auto output = engine.try_run(Input{20});
  assert(output.has_value());
  assert(output.value().value == 42);
  assert(recorder.size() == 4);
  assert(recorder.events()[0].kind == pb::runtime::trace_event_kind::stage_start);
  assert(recorder.events()[0].stage.name == "add_one");
  assert(!recorder.events()[0].has_error);
  assert(recorder.events()[3].kind == pb::runtime::trace_event_kind::stage_success);
  assert(recorder.events()[3].stage.key == "math.double");

  const auto success_json = pb::runtime::export_json(recorder);
  assert(contains(success_json, R"("schema_version":"pb.runtime.trace.v1")"));
  assert(contains(success_json, R"("kind":"stage_start")"));
  assert(contains(success_json, R"("kind":"stage_success")"));
  assert(contains(success_json, R"("key":"math.double")"));

  recorder.clear();
  auto failed = engine.try_run(Input{-1});
  assert(!failed.has_value());
  assert(recorder.size() == 4);
  assert(recorder.events()[3].kind == pb::runtime::trace_event_kind::stage_failure);
  assert(recorder.events()[3].has_error);
  assert(recorder.events()[3].diagnostic.message == "zero \"middle\"\nline");

  const auto failure_json = pb::runtime::export_json(recorder);
  assert(contains(failure_json, R"("kind":"stage_failure")"));
  assert(contains(failure_json, R"("category":"stage_failure")"));
  assert(contains(failure_json, R"("message":"zero \"middle\"\nline")"));

  return 0;
}
