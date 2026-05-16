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

using public_branch_case = pb::case_<predicate>::then<parse>;
using public_branch_node = pb::branch_node<public_branch_case>;
} // namespace

static_assert(std::is_same_v<public_branch_case::predicate_type, predicate>);
static_assert(std::is_same_v<public_branch_case::stage_type, parse>);
static_assert(std::is_same_v<public_branch_case::input_type, raw>);
static_assert(public_branch_node::case_count == 1);

int pb_public_header_pipeline() { return 0; }
