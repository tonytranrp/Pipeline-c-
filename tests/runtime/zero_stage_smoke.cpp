#include <pb/pipeline.hpp>

#include <type_traits>

struct Input {
  int value{};
};

using Pipeline = pb::from<Input>::to<Input>;
using Traits = pb::pipeline_traits<Pipeline>;
static_assert(pb::valid<Pipeline>);
static_assert(Traits::empty);
static_assert(pb::pipeline_empty_v<Pipeline>);
static_assert(Traits::stage_count == 0);
static_assert(std::same_as<Traits::input_type, Input>);
static_assert(std::same_as<Traits::output_type, Input>);
static_assert(std::same_as<pb::pipeline_input_t<Pipeline>, Input>);
static_assert(std::same_as<pb::pipeline_output_t<Pipeline>, Input>);
static_assert(std::same_as<pb::pipeline_stages_t<Pipeline>, pb::meta::type_list<>>);
static_assert(pb::describe<Pipeline>().stage_names().empty());
static_assert(pb::describe<Pipeline>().stage_records().empty());

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto output = engine.run(Input{17});
  if (output.value != 17) {
    return 1;
  }

  auto safe_output = engine.try_run(Input{23});
  if (!safe_output.has_value() || safe_output.value().value != 23) {
    return 1;
  }

  return 0;
}
