#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Decision {};

struct IsParsed {};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
};

struct Review {
  using input_type = Parsed;
  using output_type = Decision;
};

// Phase 5 branch/join markers are available, but the linear builder must still
// reject branch topology until validation/runtime support is implemented.
using UnsupportedBranch =
    pb::from<Raw>::branch<pb::case_<IsParsed>::then<Parse>, pb::case_<IsParsed>::then<Review>>;

int main() { return 0; }
