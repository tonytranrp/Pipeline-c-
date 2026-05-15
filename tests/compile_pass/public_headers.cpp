#include <pb/pipeline.hpp>

#include <array>
#include <string_view>
#include <type_traits>

struct Raw { int value{}; };
struct Parsed { int value{}; };
struct Done { int value{}; };
struct ParseError {};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  using error_type = ParseError;
  static constexpr auto name = pb::fixed_string{"parse"};
  Parsed operator()(Raw raw) const { return {raw.value + 1}; }
};

struct Finish {
  using input_type = Parsed;
  using output_type = Done;
  static constexpr auto stage_name() noexcept { return "finish"; }
  Done operator()(Parsed parsed) const { return {parsed.value * 2}; }
};

using Pipeline = pb::from<Raw>::then<Parse>::then<Finish>::to<Done>;
using Traits = pb::pipeline_traits<Pipeline>;

static_assert(pb::Stage<Parse>);
static_assert(pb::Connectable<Raw, Parse>);
static_assert(pb::ValidPipeline<Pipeline>);
static_assert(pb::valid<Pipeline>);
static_assert(pb::is_pipeline_v<Pipeline>);
static_assert(std::same_as<pb::stage_info<Parse>::input_type, Raw>);
static_assert(std::same_as<pb::stage_input_t<Parse>, Raw>);
static_assert(std::same_as<pb::stage_output_t<Parse>, Parsed>);
static_assert(std::same_as<pb::stage_error_t<Parse>, ParseError>);
static_assert(pb::stage_traits<Parse>::name() == std::string_view{"parse"});
static_assert(Traits::stage_count == 2);
static_assert(pb::pipeline_size_v<Pipeline> == 2);
static_assert(!pb::pipeline_empty_v<Pipeline>);
static_assert(std::same_as<pb::pipeline_input_t<Pipeline>, Raw>);
static_assert(std::same_as<pb::pipeline_output_t<Pipeline>, Done>);
static_assert(std::same_as<pb::pipeline_stages_t<Pipeline>, pb::meta::type_list<Parse, Finish>>);
static_assert(std::same_as<pb::pipeline_stage_t<Pipeline, 0>, Parse>);
static_assert(std::same_as<pb::pipeline_stage_descriptor_t<Pipeline, 1>, Traits::stage<1>>);
static_assert(std::same_as<pb::stage_descriptor<0, Parse>::input_type, Raw>);
static_assert(pb::pipeline_descriptor<Pipeline>::stage_count == 2);
static_assert(pb::describe<Pipeline>().stage_name<0>() == std::string_view{"parse"});
static_assert(pb::describe<Pipeline>().stage_name<1>() == std::string_view{"finish"});

int main() {
  constexpr auto desc = pb::describe<Pipeline>();
  constexpr auto names = desc.stage_names();
  constexpr auto records = desc.stage_records();
  static_assert(records[0].index == 0);
  static_assert(records[1].name == std::string_view{"finish"});
  return names == std::array<std::string_view, 2>{"parse", "finish"} ? 0 : 1;
}
