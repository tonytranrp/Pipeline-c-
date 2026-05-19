#include <pb/pipeline.hpp>

#include <string>
#include <vector>

namespace {

struct Input {
  int mask{};
  int value{};
};

struct Parsed { int value{}; };
struct Reviewed { int value{}; };
struct Done { std::string text{}; };

struct IsA {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 0b001) != 0; }
};
struct IsB {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 0b010) != 0; }
};
struct IsC {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input& input) const { return (input.mask & 0b100) != 0; }
};

struct ParseA {
  using input_type = Input;
  using output_type = Parsed;
  Parsed operator()(Input input) const { return Parsed{input.value + 10}; }
};
struct Review {
  using input_type = Input;
  using output_type = Reviewed;
  Reviewed operator()(Input input) const { return Reviewed{input.value + 100}; }
};
struct ParseC {
  using input_type = Input;
  using output_type = Parsed;
  Parsed operator()(Input input) const { return Parsed{input.value + 1000}; }
};

using CaseA = pb::case_<IsA>::then<ParseA>;
using CaseB = pb::case_<IsB>::then<Review>;
using CaseC = pb::case_<IsC>::then<ParseC>;
using FanInInput = pb::fan_in_output_t<CaseA, CaseB, CaseC>;

struct FanInJoin {
  using input_type = FanInInput;
  using output_type = Done;

  Done operator()(FanInInput input) const {
    std::string text;
    text += input.template get<0>().selected() ? "A=" + std::to_string(input.template get<0>().get().value) : "A=skip";
    text += ";";
    text += input.template get<1>().selected() ? "B=" + std::to_string(input.template get<1>().get().value) : "B=skip";
    text += ";";
    text += input.template get<2>().selected() ? "C=" + std::to_string(input.template get<2>().get().value) : "C=skip";
    return Done{std::move(text)};
  }
};

using Pipeline = pb::from<Input>::branch<CaseA, CaseB, CaseC>::fan_in<FanInJoin>::to<Done>;
static_assert(pb::valid<Pipeline>);

struct CountingStage {
  using input_type = Input;
  using output_type = Parsed;
  int count{};
  Parsed operator()(Input input) { return Parsed{input.value + (++count)}; }
};
struct StatefulJoin {
  using input_type = pb::fan_in_results<pb::fan_in_case_result<0, Parsed>>;
  using output_type = Done;
  Done operator()(input_type input) const { return Done{std::to_string(input.template get<0>().get().value)}; }
};
using StatefulCase = pb::case_<IsA>::then<CountingStage>;
using StatefulPipeline = pb::from<Input>::branch<StatefulCase>::fan_in<StatefulJoin>::to<Done>;

struct RecordingObserver : pb::runtime::observer {
  std::vector<std::string> events;
  void on_case_selected(const pb::runtime::stage_id&, std::size_t case_index,
                        const pb::runtime::stage_id&) override {
    events.push_back("selected:" + std::to_string(case_index));
  }
  void on_case_skipped(const pb::runtime::stage_id&, std::size_t case_index,
                       const pb::runtime::stage_id&) override {
    events.push_back("skipped:" + std::to_string(case_index));
  }
};

bool contains(const std::vector<std::string>& events, const std::string& event) {
  for (const auto& item : events) {
    if (item == event) return true;
  }
  return false;
}

template <class Result>
bool expect_done(const Result& actual, const std::string& expected) {
  return actual.has_value() && actual.value().text == expected;
}

} // namespace

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto zero = engine.run(Input{0, 5});
  if (!expect_done(zero, "A=skip;B=skip;C=skip")) return 1;

  auto one = engine.run(Input{0b010, 7});
  if (!expect_done(one, "A=skip;B=107;C=skip")) return 1;

  RecordingObserver observer;
  engine.set_observer(&observer);
  auto many = engine.run(Input{0b101, 3});
  if (!expect_done(many, "A=13;B=skip;C=1003")) return 1;
  if (!contains(observer.events, "selected:0")) return 1;
  if (!contains(observer.events, "skipped:1")) return 1;
  if (!contains(observer.events, "selected:2")) return 1;

  auto stateful = pb::compile<StatefulPipeline>(pb::runtime::stateful_sequential{});
  auto first = stateful.run(Input{0b001, 20});
  auto second = stateful.run(Input{0b001, 20});
  if (!expect_done(first, "21")) return 1;
  if (!expect_done(second, "22")) return 1;

  return 0;
}
