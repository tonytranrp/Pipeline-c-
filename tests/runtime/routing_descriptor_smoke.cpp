#include <pb/pipeline.hpp>

#include <array>
#include <cassert>
#include <string_view>

int main() {
  const auto descriptor = pb::make_route_descriptor(
      std::array{pb::route_case_record{.index = 0, .key = "parse", .name = "parse_order"},
                 pb::route_case_record{.index = 1, .key = "review", .name = "manual_review"},
                 pb::route_case_record{.index = 2, .key = "reject", .name = "reject_order"}});

  static_assert(decltype(descriptor)::case_count == 3);
  assert(descriptor.case_records()[2].key == std::string_view{"reject"});

  const auto selected = pb::select_route(descriptor, std::array{false, true, false});
  assert(selected.has_value());
  assert(selected.value().matched);
  assert(selected.value().selected.index == 1);
  assert(selected.value().selected.key == "review");

  const auto first_match_wins = pb::select_route(descriptor, std::array{true, true, false});
  assert(first_match_wins.has_value());
  assert(first_match_wins.value().selected.index == 0);

  const auto no_match = pb::select_route(descriptor, std::array{false, false, false});
  assert(no_match.has_error());
  assert(no_match.error().category == pb::error_category::contract_violation);
  assert(no_match.error().message == "no branch route matched");

  const auto mismatch = pb::select_route(descriptor, std::array{true, false});
  assert(mismatch.has_error());
  assert(mismatch.error().category == pb::error_category::contract_violation);
  assert(mismatch.error().message == "branch route predicate count mismatch");
}
