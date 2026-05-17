#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Decision {};

struct IsParsed {
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
  using output_type = Decision;
};

// Phase 5 branch/join markers are available, but the linear builder must still
// reject branch topology until validation/runtime support is implemented.
using UnsupportedBranch =
    pb::from<Raw>::branch<pb::case_<IsParsed>::then<Parse>, pb::case_<NeedsReview>::then<Review>>;
static_assert(sizeof(UnsupportedBranch) > 0);

int main() { return 0; }
