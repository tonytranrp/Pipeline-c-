#include <pb/pipeline.hpp>

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
struct join {
  using input_type = parsed;
  using output_type = parsed;
};

using public_branch_case = pb::case_<predicate>::then<parse>;
using public_branch_case_output = pb::branch_case_output<public_branch_case>;
using public_branch_node = pb::branch_node<public_branch_case>;
using public_branch_outputs = pb::branch_outputs<public_branch_case>;
using public_join_node = pb::join_node<join>;
} // namespace

static_assert(std::is_same_v<public_branch_case::predicate_type, predicate>);
static_assert(std::is_same_v<public_branch_case::stage_type, parse>);
static_assert(std::is_same_v<public_branch_case::input_type, raw>);
static_assert(std::is_same_v<public_branch_case_output::case_type, public_branch_case>);
static_assert(std::is_same_v<public_branch_case_output::output_type, parsed>);
static_assert(public_branch_node::case_count == 1);
static_assert(public_branch_outputs::output_count == 1);
static_assert(std::is_same_v<public_join_node::stage_type, join>);
static_assert(std::is_same_v<public_join_node::input_type, parsed>);
static_assert(std::is_same_v<public_join_node::output_type, parsed>);

int pb_public_header_pipeline() { return 0; }
