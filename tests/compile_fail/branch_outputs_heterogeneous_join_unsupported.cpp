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

// Current sequential branch execution supports homogeneous branch output types
// (all branch cases must produce the same output_type). Heterogeneous branch
// outputs that produce different types (Parsed vs Reviewed) and would require a
// type_list/variant-style join input are not yet supported.
//
// This test verifies that the builder correctly rejects heterogeneous branch
// outputs at compile time via branch_output_validation.
using UnsupportedBranchJoinExecution = pb::from<Raw>::branch<ParseCase, ReviewCase>;
static_assert(sizeof(UnsupportedBranchJoinExecution) > 0);

int main() { return 0; }
