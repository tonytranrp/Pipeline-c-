#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Receipt {};
struct User {
  Receipt operator()(Parsed) const { return {}; }
};

Raw parse(std::string) { return {}; }

struct Parser {
  Receipt operator()(Parsed) const { return {}; }
};

struct Stage {
  using input_type = Raw;
  using output_type = Parsed;
  Parsed operator()(Raw) const { return {}; }
};

using StageFn = pb::adapt_fn<&parse, std::string, Raw>;
using StageMember = pb::adapt_member<&User::operator(), Parsed, Receipt>;
using StageFunctor = pb::adapt_functor<Parser, Parsed, Receipt>;

using Pipeline = pb::from<std::string>::then<StageFn>::then<Stage>::then<StageMember>::to<Receipt>;

static_assert(pb::ValidPipeline<Pipeline>);
static_assert(pb::connectable_v<std::string, StageFn>);
static_assert(pb::connectable_v<Raw, Stage>);
static_assert(pb::connectable_v<Parsed, StageMember>);
static_assert(pb::adapted_stage<StageFn>);
static_assert(pb::adapted_stage<StageMember>);
static_assert(pb::adapted_stage<StageFunctor>);
