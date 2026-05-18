#include <pb/pipeline.hpp>

#include <cassert>
#include <string>
#include <string_view>
#include <vector>

// ---------------------------------------------------------------------------
// Linear pipeline stages (existing coverage)
// ---------------------------------------------------------------------------

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

using LinearPipeline = pb::from<Input>::then<AddOne>::then<CheckedDouble>::to<Output>;
static_assert(pb::valid<LinearPipeline>);

// ---------------------------------------------------------------------------
// Branch pipeline stages (new coverage)
// ---------------------------------------------------------------------------

struct Raw {
  int value{};
};

struct Routed {
  int value{};
};

struct IsEven {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_name() noexcept { return "is_even"; }
  static constexpr auto stage_key() noexcept { return "pred.even"; }
  bool operator()(Raw raw) const { return raw.value % 2 == 0; }
};

struct IsOdd {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_name() noexcept { return "is_odd"; }
  static constexpr auto stage_key() noexcept { return "pred.odd"; }
  bool operator()(Raw raw) const { return raw.value % 2 != 0; }
};

struct EvenRoute {
  using input_type = Raw;
  using output_type = Routed;
  static constexpr auto stage_name() noexcept { return "even_route"; }
  static constexpr auto stage_key() noexcept { return "route.even"; }
  Routed operator()(Raw raw) const { return {raw.value * 10}; }
};

struct OddRoute {
  using input_type = Raw;
  using output_type = Routed;
  static constexpr auto stage_name() noexcept { return "odd_route"; }
  static constexpr auto stage_key() noexcept { return "route.odd"; }
  Routed operator()(Raw raw) const { return {raw.value * 100}; }
};

using EvenCase = pb::case_<IsEven>::then<EvenRoute>;
using OddCase = pb::case_<IsOdd>::then<OddRoute>;
using BranchPipeline = pb::from<Raw>::branch<EvenCase, OddCase>::to<Routed>;
static_assert(pb::valid<BranchPipeline>);

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

[[nodiscard]] auto contains(std::string_view haystack, std::string_view needle) -> bool {
  return haystack.find(needle) != std::string_view::npos;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main() {
  // -----------------------------------------------------------------------
  // Linear pipeline — success path
  // -----------------------------------------------------------------------
  {
    auto engine = pb::compile<LinearPipeline>(pb::runtime::sequential{});
    pb::runtime::trace_recorder recorder{};
    pb::runtime::trace_observer observer{recorder};
    engine.set_observer(&observer);

    auto output = engine.try_run(Input{20});
    assert(output.has_value());
    assert(output.value().value == 42);
    assert(recorder.size() == 4);
    assert(recorder.events()[0].kind == pb::runtime::trace_event_kind::stage_start);
    assert(recorder.events()[0].stage_name == "add_one");
    assert(!recorder.events()[0].has_error);
    assert(recorder.events()[3].kind == pb::runtime::trace_event_kind::stage_success);
    assert(recorder.events()[3].stage_key == "math.double");

    const auto success_json = pb::runtime::export_json(recorder);
    assert(contains(success_json, R"("schema_version":"pb.trace.v1")"));
    assert(contains(success_json, R"("kind":"stage_start")"));
    assert(contains(success_json, R"("kind":"stage_success")"));
    assert(contains(success_json, R"("key":"math.double")"));
  }

  // -----------------------------------------------------------------------
  // Linear pipeline — failure path
  // -----------------------------------------------------------------------
  {
    auto engine = pb::compile<LinearPipeline>(pb::runtime::sequential{});
    pb::runtime::trace_recorder recorder{};
    pb::runtime::trace_observer observer{recorder};
    engine.set_observer(&observer);

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
  }

  // -----------------------------------------------------------------------
  // Branch pipeline — even case (case_selected)
  // -----------------------------------------------------------------------
  {
    auto engine = pb::compile<BranchPipeline>(pb::runtime::sequential{});
    pb::runtime::trace_recorder recorder{};
    pb::runtime::trace_observer observer{recorder};
    engine.set_observer(&observer);

    auto even = engine.run(Raw{4});
    assert(even.has_value());
    assert(even.value().value == 40);

    // Verify case_selected and case_skipped events
    bool saw_case_selected = false;
    bool saw_case_skipped = false;
    for (const auto& event : recorder.events()) {
      if (event.kind == pb::runtime::trace_event_kind::case_selected) {
        saw_case_selected = true;
        assert(event.stage_key == "branch");
        assert(event.case_index == 0);
      }
      if (event.kind == pb::runtime::trace_event_kind::case_skipped) {
        saw_case_skipped = true;
        assert(event.stage_key == "branch");
        assert(event.case_index == 0);
      }
    }
    // Even case should be selected (not skipped) since index 0 matches
    assert(saw_case_selected);
    assert(!saw_case_skipped);

    const auto json = pb::runtime::export_json(recorder);
    assert(contains(json, R"("kind":"case_selected")"));
    assert(contains(json, "\"key\":\"branch\""));
    assert(contains(json, "\"stage_index\":0"));
    assert(contains(json, "\"case_index\":0"));
  }

  // -----------------------------------------------------------------------
  // Branch pipeline — odd case (case_skipped then case_selected)
  // -----------------------------------------------------------------------
  {
    auto engine = pb::compile<BranchPipeline>(pb::runtime::sequential{});
    pb::runtime::trace_recorder recorder{};
    pb::runtime::trace_observer observer{recorder};
    engine.set_observer(&observer);

    auto odd = engine.run(Raw{3});
    assert(odd.has_value());
    assert(odd.value().value == 300);

    // Case 0 (IsEven) should be skipped, case 1 (IsOdd) selected
    bool saw_case_0_skipped = false;
    bool saw_case_1_selected = false;
    for (const auto& event : recorder.events()) {
      if (event.kind == pb::runtime::trace_event_kind::case_skipped &&
          event.case_index == 0) {
        saw_case_0_skipped = true;
      }
      if (event.kind == pb::runtime::trace_event_kind::case_selected &&
          event.case_index == 1) {
        saw_case_1_selected = true;
      }
    }
    assert(saw_case_0_skipped);
    assert(saw_case_1_selected);

    const auto json = pb::runtime::export_json(recorder);
    assert(contains(json, R"("kind":"case_skipped")"));
    assert(contains(json, R"("kind":"case_selected")"));
  }

  // -----------------------------------------------------------------------
  // Trace recorder empty/clear round-trip
  // -----------------------------------------------------------------------
  {
    pb::runtime::trace_recorder recorder{};
    assert(recorder.empty());
    assert(recorder.size() == 0);

    recorder.write(pb::runtime::trace_event{
        .kind = pb::runtime::trace_event_kind::pipeline_start,
        .stage_key = "test",
        .stage_name = "Test Pipeline"});
    assert(!recorder.empty());
    assert(recorder.size() == 1);

    recorder.clear();
    assert(recorder.empty());
    assert(recorder.size() == 0);
  }

  // -----------------------------------------------------------------------
  // trace_event_kind_name coverage
  // -----------------------------------------------------------------------
  {
    using namespace pb::runtime;
    assert(trace_event_kind_name(trace_event_kind::stage_start) == "stage_start");
    assert(trace_event_kind_name(trace_event_kind::stage_success) == "stage_success");
    assert(trace_event_kind_name(trace_event_kind::stage_failure) == "stage_failure");
    assert(trace_event_kind_name(trace_event_kind::stage_exception) == "stage_exception");
    assert(trace_event_kind_name(trace_event_kind::case_selected) == "case_selected");
    assert(trace_event_kind_name(trace_event_kind::case_skipped) == "case_skipped");
    assert(trace_event_kind_name(trace_event_kind::pipeline_start) == "pipeline_start");
    assert(trace_event_kind_name(trace_event_kind::pipeline_complete) == "pipeline_complete");
  }

  // -----------------------------------------------------------------------
  // trace_observer pointer constructor
  // -----------------------------------------------------------------------
  {
    pb::runtime::trace_recorder recorder{};
    pb::runtime::trace_observer observer{&recorder};
    assert(observer.get_sink() == &recorder);

    observer.set_sink(nullptr);
    assert(observer.get_sink() == nullptr);

    observer.set_sink(&recorder);
    assert(observer.get_sink() == &recorder);
  }

  // -----------------------------------------------------------------------
  // JSON export on empty recorder
  // -----------------------------------------------------------------------
  {
    pb::runtime::trace_recorder recorder{};
    const auto json = pb::runtime::export_json(recorder);
    assert(contains(json, R"("schema_version":"pb.trace.v1")"));
    assert(contains(json, R"("events":[)"));
    // No events, so events list should be empty
    assert(contains(json, R"str("events":[])str"));
  }

  return 0;
}
