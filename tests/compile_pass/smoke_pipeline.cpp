#include <pb/pipeline.hpp>

#include <concepts>

namespace {

struct parse_order {
  static constexpr auto name = pb::fixed_string{"parse_order"};
  using input_type = const char*;
  using output_type = int;
};

struct validate_order {
  static constexpr auto name = pb::fixed_string{"validate_order"};
  using input_type = int;
  using output_type = long;
};

struct persist_order {
  static constexpr auto name = pb::fixed_string{"persist_order"};
  using input_type = long;
  using output_type = void;
};

using order_pipeline =
    pb::from<const char*>::then<parse_order>::to<validate_order>::then<persist_order>::chain;

static_assert(pb::core::meta::type_list_size_v<pb::core::meta::type_list<int, double, char>> == 3);
static_assert(pb::core::meta::contains<pb::core::meta::type_list<int, double>, double>::value);
static_assert(std::same_as<pb::core::meta::at_t<pb::core::meta::type_list<int, double>, 1>, double>);
static_assert(pb::core::stage<parse_order>);
static_assert(pb::core::connectable<parse_order, validate_order>);
static_assert(pb::core::stage_to_sink<validate_order, persist_order>);
static_assert(pb::validate_chain_v<const char*, parse_order, validate_order, persist_order>);
static_assert(pb::core::validate<order_pipeline>::value);
static_assert(pb::core::is_valid_chain_v<order_pipeline>);
static_assert(pb::core::stage_traits<parse_order>::name.view() == "parse_order");

}  // namespace

int main() {
  return 0;
}
