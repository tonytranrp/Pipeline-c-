#include "pb/runtime/error.hpp"

#include <ostream>
#include <string>

namespace pb::runtime {

auto category_name(error_category category) noexcept -> std::string_view {
  switch (category) {
  case error_category::stage_failure:
    return "stage_failure";
  case error_category::expected_error:
    return "expected_error";
  case error_category::exception:
    return "exception";
  case error_category::contract_violation:
    return "contract_violation";
  }
  return "unknown";
}

auto has_stage(const stage_id& stage) noexcept -> bool {
  return !stage.key.empty() || !stage.name.empty();
}

auto has_message(const error& value) noexcept -> bool {
  return !value.message.empty();
}

auto describe(const stage_id& stage) -> std::string {
  if (stage.name.empty()) {
    return has_stage(stage) ? stage.key : std::string{"<unknown stage>"};
  }
  if (stage.key.empty()) {
    return stage.name;
  }
  return stage.name + " (" + stage.key + ")";
}

auto describe(const error& value) -> std::string {
  auto text = std::string{category_name(value.category)};
  text += " at ";
  text += describe(value.stage);
  if (has_message(value)) {
    text += ": ";
    text += value.message;
  }
  return text;
}

auto operator<<(std::ostream& stream, error_category category) -> std::ostream& {
  return stream << category_name(category);
}

auto operator<<(std::ostream& stream, const stage_id& stage) -> std::ostream& {
  return stream << describe(stage);
}

auto operator<<(std::ostream& stream, const error& value) -> std::ostream& {
  return stream << describe(value);
}

} // namespace pb::runtime
