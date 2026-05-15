#include <pb/runtime/error.hpp>

#include <cassert>
#include <sstream>
#include <string>

int main() {
  using pb::runtime::category_name;
  using pb::runtime::describe;
  using pb::runtime::error;
  using pb::runtime::error_category;
  using pb::runtime::stage_id;

  assert(category_name(error_category::stage_failure) == "stage_failure");
  assert(category_name(error_category::expected_error) == "expected_error");
  assert(category_name(error_category::exception) == "exception");
  assert(category_name(error_category::contract_violation) == "contract_violation");

  assert(describe(stage_id{}) == "<unknown stage>");
  assert(describe(stage_id{.key = "parse"}) == "parse");
  assert(describe(stage_id{.name = "ParseOrder"}) == "ParseOrder");
  assert(describe(stage_id{.key = "parse", .name = "ParseOrder"}) == "ParseOrder (parse)");

  auto diagnostic = describe(error{.stage = {.key = "validate", .name = "ValidateOrder"},
                                   .category = error_category::exception,
                                   .message = "bad order"});
  assert(diagnostic == "exception at ValidateOrder (validate): bad order");

  auto message_less = describe(error{.category = error_category::contract_violation});
  assert(message_less == "contract_violation at <unknown stage>");

  std::ostringstream stream;
  stream << error_category::expected_error << " | "
         << stage_id{.key = "persist", .name = "PersistOrder"} << " | "
         << error{.stage = {.key = "save", .name = "SaveOrder"},
                  .category = error_category::stage_failure,
                  .message = "disk full"};
  assert(stream.str() == "expected_error | PersistOrder (persist) | stage_failure at SaveOrder (save): disk full");

  return 0;
}
