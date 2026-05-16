#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Normalized {};
struct Joined {};

struct IsParsedPath {
  using input_type = Raw;
  using output_type = bool;
};

struct IsNormalizedPath {
  using input_type = Raw;
  using output_type = bool;
};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
};

struct Normalize {
  using input_type = Raw;
  using output_type = Normalized;
};

struct BadJoin {
  using input_type = Parsed;
  using output_type = Joined;
};

using ParsedCase = pb::case_<IsParsedPath>::then<Parse>;
using NormalizedCase = pb::case_<IsNormalizedPath>::then<Normalize>;
using Outputs = pb::branch_outputs<ParsedCase, NormalizedCase>;
using Join = pb::join_node<BadJoin>;

// Join validation expects the join stage input_type to consume the full
// branch output type-list, not one individual branch output.
using BadValidation = pb::join_validation<Outputs, Join>;
static_assert(sizeof(BadValidation) > 0);

int main() { return 0; }
