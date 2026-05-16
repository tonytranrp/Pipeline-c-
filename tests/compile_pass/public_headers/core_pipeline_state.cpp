#include <pb/core/pipeline_state.hpp>

#include <type_traits>

namespace {
struct raw {};
struct parsed {};
struct predicate {
  using input_type = raw;
  using output_type = bool;
};
struct parse {
  using input_type = raw;
  using output_type = parsed;
};
struct reviewed {};
struct manual_review {
  using input_type = raw;
  using output_type = reviewed;
};
struct join {
  using input_type = parsed;
  using output_type = parsed;
};

using branch_case = pb::core::case_<predicate>::then<parse>;
using review_branch_case = pb::core::case_<predicate>::then<manual_review>;
using branch_case_output = pb::core::branch_case_output<branch_case>;
using branch_node = pb::core::branch_node<branch_case, review_branch_case>;
using branch_outputs = pb::core::branch_outputs<branch_case, review_branch_case>;
using join_node = pb::core::join_node<join>;
using join_output = pb::core::join_output<join_node>;
} // namespace

static_assert(std::is_same_v<branch_case::predicate_type, predicate>);
static_assert(std::is_same_v<branch_case::stage_type, parse>);
static_assert(std::is_same_v<branch_case::input_type, raw>);
static_assert(std::is_same_v<branch_case_output::case_type, branch_case>);
static_assert(std::is_same_v<branch_case_output::output_type, parsed>);
static_assert(branch_node::case_count == 2);
static_assert(std::is_same_v<branch_node::input_type, raw>);
static_assert(std::is_same_v<branch_node::cases, pb::core::meta::type_list<branch_case, review_branch_case>>);
static_assert(std::is_same_v<branch_node::output_types, pb::core::meta::type_list<parsed, reviewed>>);
static_assert(branch_outputs::output_count == 2);
static_assert(std::is_same_v<branch_outputs::input_type, raw>);
static_assert(std::is_same_v<branch_outputs::cases, pb::core::meta::type_list<branch_case, review_branch_case>>);
static_assert(std::is_same_v<branch_outputs::output_types, pb::core::meta::type_list<parsed, reviewed>>);
static_assert(std::is_same_v<join_node::stage_type, join>);
static_assert(std::is_same_v<join_node::input_type, parsed>);
static_assert(std::is_same_v<join_node::output_type, parsed>);
static_assert(std::is_same_v<join_output::join_type, join_node>);
static_assert(std::is_same_v<join_output::stage_type, join>);
static_assert(std::is_same_v<join_output::input_type, parsed>);
static_assert(std::is_same_v<join_output::output_type, parsed>);

int pb_public_header_core_pipeline_state() { return 0; }
