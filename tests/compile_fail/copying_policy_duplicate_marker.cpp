#include <pb/pipeline.hpp>

struct Raw {
  int value{};
};

struct Done {
  int value{};
};

struct Stage {
  using input_type = Raw;
  using output_type = Done;
  Done operator()(Raw input) const { return {.value = input.value + 1}; }
};

using Broken = pb::from<Raw>::with<pb::policy::copying::move_only>::with<
    pb::policy::copying::move_only>::then<Stage>::to<Done>;

static_assert(pb::valid<Broken>);

int main() { return 0; }
