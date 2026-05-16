#include <pb/pipeline.hpp>

struct Raw {};
struct Parsed {};
struct Done {};
struct NotAStage {};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  Parsed operator()(Raw) const { return {}; }
};

struct Finish {
  using input_type = Parsed;
  using output_type = Done;
  Done operator()(Parsed) const { return {}; }
};

struct MetadataOnlyStage {
  using input_type = Raw;
  using output_type = Parsed;
};

static_assert(pb::Stage<Parse>);
static_assert(pb::stage<Parse>);
static_assert(!pb::Stage<NotAStage>);
static_assert(!pb::stage<NotAStage>);

static_assert(pb::Connectable<Raw, Parse>);
static_assert(pb::connectable_v<Raw, Parse>);
static_assert(!pb::Connectable<Parsed, Parse>);
static_assert(!pb::connectable_v<Parsed, Parse>);
static_assert(!pb::Connectable<Raw, NotAStage>);
static_assert(!pb::connectable_v<Raw, NotAStage>);

static_assert(pb::AdjacentStages<Parse, Finish>);
static_assert(pb::adjacent_stages_v<Parse, Finish>);
static_assert(!pb::AdjacentStages<Finish, Parse>);
static_assert(!pb::adjacent_stages_v<Finish, Parse>);
static_assert(!pb::AdjacentStages<NotAStage, Finish>);
static_assert(!pb::adjacent_stages_v<NotAStage, Finish>);
static_assert(!pb::AdjacentStages<Parse, NotAStage>);
static_assert(!pb::adjacent_stages_v<Parse, NotAStage>);

static_assert(pb::RunnableStage<Parse, Raw>);
static_assert(pb::runnable_stage_v<Parse, Raw>);
static_assert(!pb::RunnableStage<Parse, Parsed>);
static_assert(!pb::runnable_stage_v<Parse, Parsed>);
static_assert(!pb::RunnableStage<MetadataOnlyStage, Raw>);
static_assert(!pb::runnable_stage_v<MetadataOnlyStage, Raw>);
static_assert(!pb::RunnableStage<NotAStage, Raw>);
static_assert(!pb::runnable_stage_v<NotAStage, Raw>);

int main() { return 0; }
