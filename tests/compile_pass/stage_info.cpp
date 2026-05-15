#include <pb/pipeline.hpp>

#include <string_view>
#include <type_traits>

struct Raw { int value{}; };
struct Parsed { int value{}; };
struct ParseError {};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  using error_type = ParseError;
  static constexpr auto stage_name() noexcept { return "parse"; }
  Parsed operator()(Raw raw) const { return {raw.value + 1}; }
};

using Info = pb::stage_info<Parse>;

static_assert(Info::valid);
static_assert(std::same_as<Info::stage_type, Parse>);
static_assert(std::same_as<Info::input_type, Raw>);
static_assert(std::same_as<Info::output_type, Parsed>);
static_assert(std::same_as<Info::error_type, ParseError>);
static_assert(Info::name() == std::string_view{"parse"});
static_assert(std::same_as<pb::stage_input_t<Parse>, Raw>);
static_assert(std::same_as<pb::stage_output_t<Parse>, Parsed>);
static_assert(std::same_as<pb::stage_error_t<Parse>, ParseError>);

int main() { return Info::name() == "parse" ? 0 : 1; }
