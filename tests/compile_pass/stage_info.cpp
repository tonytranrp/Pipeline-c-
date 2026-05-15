#include <pb/pipeline.hpp>

#include <string_view>
#include <type_traits>

struct Raw { int value{}; };
struct Parsed { int value{}; };
struct ParseError {};
struct NotAStage {};

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  using error_type = ParseError;
  static constexpr auto stage_key() noexcept { return "order.parse"; }
  static constexpr auto stage_name() noexcept { return "parse"; }
  Parsed operator()(Raw raw) const { return {raw.value + 1}; }
};

using Info = pb::stage_info<Parse>;

static_assert(Info::valid);
static_assert(!pb::stage_info<NotAStage>::valid);
static_assert(pb::stage_info<NotAStage>::name() == std::string_view{"<invalid>"});
static_assert(pb::stage_info<NotAStage>::key() == std::string_view{"<invalid>"});
static_assert(std::same_as<Info::stage_type, Parse>);
static_assert(std::same_as<Info::input_type, Raw>);
static_assert(std::same_as<Info::output_type, Parsed>);
static_assert(std::same_as<Info::error_type, ParseError>);
static_assert(Info::key() == std::string_view{"order.parse"});
static_assert(Info::name() == std::string_view{"parse"});
static_assert(std::same_as<pb::stage_input_t<Parse>, Raw>);
static_assert(std::same_as<pb::stage_output_t<Parse>, Parsed>);
static_assert(std::same_as<pb::stage_error_t<Parse>, ParseError>);
static_assert(pb::stage_key<Parse>() == std::string_view{"order.parse"});
static_assert(pb::stage_name<Parse>() == std::string_view{"parse"});

int main() { return Info::name() == "parse" ? 0 : 1; }
