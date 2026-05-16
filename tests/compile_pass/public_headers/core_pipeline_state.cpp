#include <pb/core/pipeline_state.hpp>

#include <type_traits>

namespace {
struct raw {};
struct parsed {};
struct predicate {};
struct parse {
  using input_type = raw;
  using output_type = parsed;
};

using branch_case = pb::core::case_<predicate>::then<parse>;
using branch_node = pb::core::branch_node<branch_case>;
} // namespace

static_assert(std::is_same_v<branch_case::predicate_type, predicate>);
static_assert(std::is_same_v<branch_case::stage_type, parse>);
static_assert(branch_node::case_count == 1);
static_assert(std::is_same_v<branch_node::cases, pb::core::meta::type_list<branch_case>>);

int pb_public_header_core_pipeline_state() { return 0; }
