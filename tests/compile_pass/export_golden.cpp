#include <pb/pipeline.hpp>

#include <string>
#include <string_view>
#include <variant>

namespace linear_json_golden {
struct Raw { int value{}; };
struct Parsed { int value{}; };
struct Done { int value{}; };

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  static constexpr auto stage_key() noexcept { return "parse"; }
  static constexpr auto stage_name() noexcept { return "Parse"; }
  Parsed operator()(Raw raw) const { return {raw.value + 1}; }
};

struct Finish {
  using input_type = Parsed;
  using output_type = Done;
  static constexpr auto stage_key() noexcept { return "finish"; }
  static constexpr auto stage_name() noexcept { return "Finish"; }
  Done operator()(Parsed parsed) const { return {parsed.value + 1}; }
};

using Pipeline = pb::from<Raw>::then<Parse>::then<Finish>::to<Done>;

constexpr auto expected =
    "{\"schema_version\":\"pb.core.graph.v1\",\"topology\":\"linear\",\"stage_count\":2,\"edge_count\":1,"
    "\"stages\":[{\"index\":0,\"key\":\"parse\",\"name\":\"Parse\",\"kind\":\"stage\"},"
    "{\"index\":1,\"key\":\"finish\",\"name\":\"Finish\",\"kind\":\"stage\"}],"
    "\"edges\":[{\"index\":0,\"from_stage_index\":0,\"to_stage_index\":1,\"from_key\":\"parse\","
    "\"from_name\":\"Parse\",\"to_key\":\"finish\",\"to_name\":\"Finish\"}]}";
} // namespace linear_json_golden

namespace homogeneous_branch_json_golden {
struct Raw { int value{}; };
struct Routed { int value{}; };
struct Done { int value{}; };

struct IsEven {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_key() noexcept { return "is.even"; }
  static constexpr auto stage_name() noexcept { return "IsEven"; }
  bool operator()(Raw raw) const { return raw.value % 2 == 0; }
};

struct IsOdd {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_key() noexcept { return "is.odd"; }
  static constexpr auto stage_name() noexcept { return "IsOdd"; }
  bool operator()(Raw raw) const { return raw.value % 2 != 0; }
};

struct EvenRoute {
  using input_type = Raw;
  using output_type = Routed;
  static constexpr auto stage_key() noexcept { return "route.even"; }
  static constexpr auto stage_name() noexcept { return "EvenRoute"; }
  Routed operator()(Raw raw) const { return {raw.value * 2}; }
};

struct OddRoute {
  using input_type = Raw;
  using output_type = Routed;
  static constexpr auto stage_key() noexcept { return "route.odd"; }
  static constexpr auto stage_name() noexcept { return "OddRoute"; }
  Routed operator()(Raw raw) const { return {raw.value * 3}; }
};

struct Finish {
  using input_type = Routed;
  using output_type = Done;
  static constexpr auto stage_key() noexcept { return "finish"; }
  static constexpr auto stage_name() noexcept { return "Finish"; }
  Done operator()(Routed routed) const { return {routed.value + 1}; }
};

using EvenCase = pb::case_<IsEven>::then<EvenRoute>;
using OddCase = pb::case_<IsOdd>::then<OddRoute>;
using Pipeline = pb::from<Raw>::branch<EvenCase, OddCase>::join<Finish>::to<Done>;

constexpr auto expected_json =
    "{\"schema_version\":\"pb.core.graph.v1\",\"topology\":\"branch\",\"stage_count\":2,\"edge_count\":1,"
    "\"stages\":[{\"index\":0,\"key\":\"branch\",\"name\":\"branch\",\"kind\":\"branch\",\"branch_cases\":["
    "{\"index\":0,\"predicate_key\":\"is.even\",\"predicate_name\":\"IsEven\",\"stage_key\":\"route.even\","
    "\"stage_name\":\"EvenRoute\",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"},"
    "\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}},"
    "{\"index\":1,\"predicate_key\":\"is.odd\",\"predicate_name\":\"IsOdd\",\"stage_key\":\"route.odd\","
    "\"stage_name\":\"OddRoute\",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"},"
    "\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}}]},"
    "{\"index\":1,\"key\":\"finish\",\"name\":\"Finish\",\"kind\":\"stage\"}],"
    "\"edges\":[{\"index\":0,\"from_stage_index\":0,\"to_stage_index\":1,\"from_key\":\"branch\","
    "\"from_name\":\"branch\",\"to_key\":\"finish\",\"to_name\":\"Finish\"}]}";

constexpr auto expected_dot = R"DOT(digraph branch_golden {
  rankdir=LR;
  node [shape=box];

  from_input [label="Input"];

  branch_0 [shape=diamond, label="branch"];

  subgraph cluster_case_0_0 {
    label="Case 0: IsEven";
    pred_0_0 [label="pred: IsEven"];
    case_0_0 [label="EvenRoute"];
    branch_0 -> pred_0_0 [style=dashed, label="test"];
    pred_0_0 -> case_0_0;
  }
  subgraph cluster_case_0_1 {
    label="Case 1: IsOdd";
    pred_0_1 [label="pred: IsOdd"];
    case_0_1 [label="OddRoute"];
    branch_0 -> pred_0_1 [style=dashed, label="test"];
    pred_0_1 -> case_0_1;
  }

  stage_1 [label="Finish\n(finish)"];
  to_output [shape=doublecircle, label="Output"];

  from_input -> branch_0;
  case_0_0 -> stage_1;
  case_0_1 -> stage_1;
  stage_1 -> to_output;
}
)DOT";
} // namespace homogeneous_branch_json_golden

namespace heterogeneous_branch_json_golden {
struct Raw { int value{}; };
struct Parsed { int value{}; };
struct Reviewed { int value{}; };
struct Done { int value{}; };

struct IsParse {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_key() noexcept { return "is.parse"; }
  static constexpr auto stage_name() noexcept { return "IsParse"; }
  bool operator()(Raw raw) const { return raw.value >= 0; }
};

struct IsReview {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_key() noexcept { return "is.review"; }
  static constexpr auto stage_name() noexcept { return "IsReview"; }
  bool operator()(Raw raw) const { return raw.value < 0; }
};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  static constexpr auto stage_key() noexcept { return "route.parse"; }
  static constexpr auto stage_name() noexcept { return "Parse"; }
  Parsed operator()(Raw raw) const { return {raw.value}; }
};

struct Review {
  using input_type = Raw;
  using output_type = Reviewed;
  static constexpr auto stage_key() noexcept { return "route.review"; }
  static constexpr auto stage_name() noexcept { return "Review"; }
  Reviewed operator()(Raw raw) const { return {raw.value}; }
};

struct Join {
  using input_type = std::variant<Parsed, Reviewed>;
  using output_type = Done;
  static constexpr auto stage_key() noexcept { return "join"; }
  static constexpr auto stage_name() noexcept { return "Join"; }
  Done operator()(std::variant<Parsed, Reviewed>) const { return {}; }
};

using ParseCase = pb::case_<IsParse>::then<Parse>;
using ReviewCase = pb::case_<IsReview>::then<Review>;
using Pipeline = pb::from<Raw>::branch<ParseCase, ReviewCase>::join<Join>::to<Done>;

constexpr auto expected =
    "{\"schema_version\":\"pb.core.graph.v1\",\"topology\":\"branch\",\"stage_count\":2,\"edge_count\":1,"
    "\"stages\":[{\"index\":0,\"key\":\"branch\",\"name\":\"branch\",\"kind\":\"branch\",\"branch_cases\":["
    "{\"index\":0,\"predicate_key\":\"is.parse\",\"predicate_name\":\"IsParse\",\"stage_key\":\"route.parse\","
    "\"stage_name\":\"Parse\",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"},"
    "\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}},"
    "{\"index\":1,\"predicate_key\":\"is.review\",\"predicate_name\":\"IsReview\",\"stage_key\":\"route.review\","
    "\"stage_name\":\"Review\",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"},"
    "\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}}]},"
    "{\"index\":1,\"key\":\"join\",\"name\":\"Join\",\"kind\":\"stage\"}],"
    "\"edges\":[{\"index\":0,\"from_stage_index\":0,\"to_stage_index\":1,\"from_key\":\"branch\","
    "\"from_name\":\"branch\",\"to_key\":\"join\",\"to_name\":\"Join\"}]}";
} // namespace heterogeneous_branch_json_golden

namespace dot_escaping {
struct Raw { int value{}; };
struct Routed { int value{}; };
struct Done { int value{}; };

struct WeirdPredicate {
  using input_type = Raw;
  using output_type = bool;
  static constexpr auto stage_key() noexcept { return "pred key/with spaces"; }
  static constexpr auto stage_name() noexcept { return "Pred \"quoted\"\\slash\nline"; }
  bool operator()(Raw) const { return true; }
};

struct WeirdRoute {
  using input_type = Raw;
  using output_type = Routed;
  static constexpr auto stage_key() noexcept { return "route key/with spaces"; }
  static constexpr auto stage_name() noexcept { return "Route \"quoted\"\\slash\nline"; }
  Routed operator()(Raw raw) const { return {raw.value}; }
};

struct WeirdFinish {
  using input_type = Routed;
  using output_type = Done;
  static constexpr auto stage_key() noexcept { return "finish key/with spaces"; }
  static constexpr auto stage_name() noexcept { return "Finish \"quoted\"\\slash\nline"; }
  Done operator()(Routed routed) const { return {routed.value}; }
};

using WeirdCase = pb::case_<WeirdPredicate>::then<WeirdRoute>;
using Pipeline = pb::from<Raw>::branch<WeirdCase>::join<WeirdFinish>::to<Done>;
} // namespace dot_escaping

namespace {
[[nodiscard]] auto expect_equal(std::string_view actual, std::string_view expected) -> bool {
  return actual == expected;
}

[[nodiscard]] auto expect_contains(std::string_view actual, std::string_view expected) -> bool {
  return actual.find(expected) != std::string_view::npos;
}
} // namespace

int main() {
  if (!expect_equal(pb::to_json<linear_json_golden::Pipeline>(), linear_json_golden::expected)) return 1;
  if (!expect_equal(pb::to_json<homogeneous_branch_json_golden::Pipeline>(),
                    homogeneous_branch_json_golden::expected_json)) return 2;
  if (!expect_equal(pb::to_json<heterogeneous_branch_json_golden::Pipeline>(),
                    heterogeneous_branch_json_golden::expected)) return 3;
  if (!expect_equal(pb::to_dot<homogeneous_branch_json_golden::Pipeline>("branch_golden"),
                    homogeneous_branch_json_golden::expected_dot)) return 4;

  const auto escaped_dot = pb::to_dot<dot_escaping::Pipeline>("escaping graph");
  if (!expect_contains(escaped_dot, "digraph escaping_graph")) return 5;
  if (!expect_contains(escaped_dot, "label=\"Case 0: Pred \\\"quoted\\\"\\\\slash\\nline\"")) return 6;
  if (!expect_contains(escaped_dot, "pred_0_0 [label=\"pred: Pred \\\"quoted\\\"\\\\slash\\nline\"]")) return 7;
  if (!expect_contains(escaped_dot, "case_0_0 [label=\"Route \\\"quoted\\\"\\\\slash\\nline\"]")) return 8;
  if (!expect_contains(escaped_dot,
                       "stage_1 [label=\"Finish \\\"quoted\\\"\\\\slash\\nline\\n(finish key/with spaces)\"]")) return 9;

  return 0;
}
