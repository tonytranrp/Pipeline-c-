#pragma once

#include <ostream>
#include <string>
#include <string_view>

namespace pb::runtime {

enum class error_category {
  stage_failure,
  expected_error,
  exception,
  contract_violation,
};

struct stage_id {
  std::string key{};
  std::string name{};
};

struct error {
  stage_id stage{};
  error_category category{error_category::stage_failure};
  std::string message{};
};

struct error_record {
  std::string stage_key{};
  std::string stage_name{};
  std::string category{};
  std::string message{};
};

[[nodiscard]] auto category_name(error_category category) noexcept -> std::string_view;
[[nodiscard]] auto has_category(const error& value, error_category category) noexcept -> bool;
[[nodiscard]] auto has_stage(const stage_id& stage) noexcept -> bool;
[[nodiscard]] auto has_stage(const error& value) noexcept -> bool;
[[nodiscard]] auto has_message(const error& value) noexcept -> bool;
[[nodiscard]] auto to_record(const error& value) -> error_record;
[[nodiscard]] auto describe(const stage_id& stage) -> std::string;
[[nodiscard]] auto describe(const error& value) -> std::string;

auto operator<<(std::ostream& stream, error_category category) -> std::ostream&;
auto operator<<(std::ostream& stream, const stage_id& stage) -> std::ostream&;
auto operator<<(std::ostream& stream, const error& value) -> std::ostream&;

} // namespace pb::runtime
