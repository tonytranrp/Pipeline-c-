#pragma once

#include <ostream>
#include <string>
#include <string_view>

#include "pb/runtime/result.hpp"

namespace pb::runtime {

[[nodiscard]] auto category_name(error_category category) noexcept -> std::string_view;
[[nodiscard]] auto has_stage(const stage_id& stage) noexcept -> bool;
[[nodiscard]] auto has_stage(const error& value) noexcept -> bool;
[[nodiscard]] auto has_message(const error& value) noexcept -> bool;
[[nodiscard]] auto describe(const stage_id& stage) -> std::string;
[[nodiscard]] auto describe(const error& value) -> std::string;

auto operator<<(std::ostream& stream, error_category category) -> std::ostream&;
auto operator<<(std::ostream& stream, const stage_id& stage) -> std::ostream&;
auto operator<<(std::ostream& stream, const error& value) -> std::ostream&;

} // namespace pb::runtime
