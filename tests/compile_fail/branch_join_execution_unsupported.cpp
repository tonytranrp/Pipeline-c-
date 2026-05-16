#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Reviewed {};
struct Joined {};

struct IsParseable {
  using input_type = Raw;
  using output_type = bool;
};

struct NeedsReview {
  using input_type = Raw;
  using output_type = bool;
};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
};

struct Review {
  using input_type = Raw;
  using output_type = Reviewed;
};

struct JoinResults {
  using input_type = pb::meta::type_list<Parsed, Reviewed>;
  using output_type = Joined;
};

using ParseCase = pb::case_<IsParseable>::then<Parse>;
using ReviewCase = pb::case_<NeedsReview>::then<Review>;
using Outputs = pb::branch_outputs<ParseCase, ReviewCase>;
using Join = pb::join_node<JoinResults>;
using CheckedJoin = pb::join_validation<Outputs, Join>;
static_assert(sizeof(CheckedJoin) > 0);

// Regression guard for the promoted sequential branch-execution scope: branch
// and join metadata can be validated, but the pipeline builder/runtime must not
// silently accept executable branch topology until Worker 1/2 product support
// lands with runtime tests.
using UnsupportedBranchJoinExecution = pb::from<Raw>::branch<ParseCase, ReviewCase>;
static_assert(sizeof(UnsupportedBranchJoinExecution) > 0);

int main() { return 0; }
