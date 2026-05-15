#pragma once

#include <string>
#include <string_view>

#include "pb/runtime/result.hpp"

namespace pb::runtime {

[[nodiscard]] auto category_name(error_category category) noexcept -> std::string_view;
[[nodiscard]] auto describe(const stage_id& stage) -> std::string;
[[nodiscard]] auto describe(const error& value) -> std::string;

} // namespace pb::runtime
