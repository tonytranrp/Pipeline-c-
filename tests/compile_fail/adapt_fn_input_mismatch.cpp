#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};

auto parse_raw(Raw) -> Parsed { return {}; }

using BadAdaptedParse = pb::adapt_fn<parse_raw, Parsed, Parsed>;
static_assert(pb::adapted_stage<BadAdaptedParse>,
              "adapted stage requires callable signature to match declared input_type");
