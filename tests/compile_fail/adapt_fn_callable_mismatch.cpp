#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Receipt {};

Parsed parse_receipt(Receipt) { return {}; }

using BrokenAdaptedStage = pb::adapt_fn<&parse_receipt, Raw, Parsed>;

static_assert(pb::Stage<BrokenAdaptedStage>);
static_assert(pb::connectable_v<Raw, BrokenAdaptedStage>);
static_assert(pb::adapted_stage<BrokenAdaptedStage>,
              "Adapted stage callable mismatch: declared input_type must be invocable by wrapped callable");
