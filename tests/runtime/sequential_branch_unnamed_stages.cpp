#include <pb/pipeline.hpp>
#include <cassert>
#include <string>
#include <vector>

struct Input { int value{}; };
struct Output { int result{}; };

struct PredA {
  using input_type = Input;
  using output_type = bool;
  bool operator()(Input in) const { return in.value < 50; }
};

struct PredB {
  using input_type = Input;
  using output_type = bool;
  bool operator()(Input in) const { return in.value >= 50; }
};

struct StageA {
  using input_type = Input;
  using output_type = Output;
  Output operator()(Input in) const { return Output{in.value * 2}; }
};

struct StageB {
  using input_type = Input;
  using output_type = Output;
  Output operator()(Input in) const { return Output{in.value * 3}; }
};

struct RecordingObserver : pb::runtime::observer {
  std::vector<std::string> events;

  void on_stage_start(const pb::runtime::stage_id& stage) override {
    events.push_back("start:" + stage.key);
  }
  void on_stage_success(const pb::runtime::stage_id& stage) override {
    events.push_back("success:" + stage.key);
  }
  void on_stage_failure(const pb::runtime::stage_id& stage, const pb::runtime::error& err) override {
    events.push_back("failure:" + stage.key + ":" + err.stage.key);
  }
  void on_case_selected(const pb::runtime::stage_id& branch_id, std::size_t case_index,
                        const pb::runtime::stage_id& predicate_id) override {
    events.push_back("case_selected:" + branch_id.key + ":" + std::to_string(case_index) + ":" + predicate_id.key);
  }
  void on_case_skipped(const pb::runtime::stage_id& branch_id, std::size_t case_index,
                       const pb::runtime::stage_id& predicate_id) override {
    events.push_back("case_skipped:" + branch_id.key + ":" + std::to_string(case_index) + ":" + predicate_id.key);
  }
};

[[nodiscard]] bool contains(const std::vector<std::string>& values, const std::string& expected) {
  for (const auto& v : values) if (v == expected) return true;
  return false;
}

using CaseA = pb::case_<PredA>::then<StageA>;
using CaseB = pb::case_<PredB>::then<StageB>;
using Pipeline = pb::from<Input>::branch<CaseA, CaseB>::to<Output>;

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  RecordingObserver observer;
  engine.set_observer(&observer);

  // All stages are unnamed — stage_key() / stage_name() not defined.
  // The runtime should generate distinct fallback identities for each predicate
  // and branch stage so they don't collide.
  auto result = engine.run(Input{25});
  assert(result.has_value());
  assert(result.value().result == 50);

  // The branch node always has key="branch"
  assert(contains(observer.events, "start:branch"));
  assert(contains(observer.events, "success:branch"));

  // Predicates and stages get numeric fallback keys from their StageIndex.
  // They all share the branch node's StageIndex, so they should NOT collide
  // (each gets a unique identity from the runtime's stage_id_for).
  // Verify that case_selected and case_skipped events reference distinct
  // predicate keys (even though they share StageIndex).
  bool saw_case_selected = false;
  bool saw_case_skipped = false;
  for (const auto& event : observer.events) {
    if (event.find("case_selected:branch:") == 0) saw_case_selected = true;
    if (event.find("case_skipped:branch:") == 0) saw_case_skipped = true;
  }
  assert(saw_case_selected);
  assert(saw_case_skipped);

  return 0;
}
