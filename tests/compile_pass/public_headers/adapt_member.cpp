#include <pb/pipeline.hpp>

struct Counter {
  int bump(int value) const { return value + 1; }
};

using BumpStage = pb::adapt_member<&Counter::bump, int, int>;

static_assert(pb::Stage<BumpStage>);
static_assert(std::same_as<decltype(BumpStage{}(41)), int>);
static_assert(pb::adapted_stage<BumpStage>);
