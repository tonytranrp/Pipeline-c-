#include <pb/runtime/fan_in_error.hpp>

#include <string_view>
#include <type_traits>

static_assert(pb::fan_in_error_schema_version == std::string_view{"pb.fan_in.errors.v1"});
static_assert(std::is_same_v<decltype(pb::fan_in_case_error::case_index), std::size_t>);
static_assert(std::is_same_v<decltype(pb::fan_in_case_error::stage), pb::runtime::stage_id>);
static_assert(std::is_same_v<decltype(pb::fan_in_case_error::diagnostic), pb::runtime::error>);

using Aggregate = pb::core::fan_in_results<pb::core::fan_in_case_result<0, int>>;
static_assert(
    std::is_same_v<decltype(pb::collect_fan_in_errors(std::declval<const Aggregate&>())),
                   pb::fan_in_error_envelope>);

int pb_public_header_runtime_fan_in_error() {
  pb::fan_in_error_envelope envelope;
  envelope.push(pb::fan_in_case_error{.case_index = 0});
  return envelope.has_failures() && !envelope.empty() && envelope.size() == 1 &&
                 envelope.begin() != envelope.end() && !envelope[0].diagnostic.message.empty()
             ? 0
             : (envelope.to_string().empty() ? 0 : 0);
}
