#include <pb/pipeline.hpp>

#include <array>
#include <string_view>
#include <type_traits>

struct Raw { int value{}; };
struct Parsed { int value{}; };
struct Done { int value{}; };

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
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
using ParseDescriptor = pb::stage_descriptor<0, Parse>;

static_assert(pb::valid<Pipeline>);
static_assert(Traits::stage_count == 2);
static_assert(!Traits::empty);
static_assert(std::same_as<Traits::input_type, Raw>);
static_assert(std::same_as<Traits::output_type, Done>);
static_assert(std::same_as<Traits::stage_type<0>, Parse>);
static_assert(std::same_as<Traits::stage<1>::input_type, Parsed>);
static_assert(ParseDescriptor::index == 0);
static_assert(std::same_as<ParseDescriptor::info, pb::stage_info<Parse>>);
static_assert(ParseDescriptor::name() == std::string_view{"parse"});
static_assert(pb::describe<Pipeline>().stage_count == 2);
static_assert(pb::describe<Pipeline>().stage_name<0>() == std::string_view{"parse"});
static_assert(pb::describe<Pipeline>().stage_name<1>() == std::string_view{"finish"});

int main() {
  constexpr auto desc = pb::describe<Pipeline>();
  constexpr auto names = desc.stage_names();
  static_assert(names.size() == 2);
  static_assert(names[0] == std::string_view{"parse"});
  static_assert(names[1] == std::string_view{"finish"});
  return names == std::array<std::string_view, 2>{"parse", "finish"} ? 0 : 1;
}
