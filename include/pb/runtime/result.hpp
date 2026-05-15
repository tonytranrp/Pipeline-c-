#pragma once

#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>

namespace pb::runtime {

struct error {
  std::string stage;
  std::string message;
};

template <class T, class E = error>
class result {
public:
  using value_type = T;
  using error_type = E;

  result(const T& value) : storage_(value) {}
  result(T&& value) : storage_(std::move(value)) {}
  result(const E& error_value) : storage_(error_value) {}
  result(E&& error_value) : storage_(std::move(error_value)) {}

  [[nodiscard]] bool has_value() const noexcept { return std::holds_alternative<T>(storage_); }
  explicit operator bool() const noexcept { return has_value(); }

  [[nodiscard]] T& value() & { return std::get<T>(storage_); }
  [[nodiscard]] const T& value() const& { return std::get<T>(storage_); }
  [[nodiscard]] T&& value() && { return std::move(std::get<T>(storage_)); }

  [[nodiscard]] E& error() & { return std::get<E>(storage_); }
  [[nodiscard]] const E& error() const& { return std::get<E>(storage_); }

private:
  std::variant<T, E> storage_;
};

template <class E>
class result<void, E> {
public:
  using value_type = void;
  using error_type = E;

  result() = default;
  result(const E& error_value) : error_(error_value), has_value_(false) {}
  result(E&& error_value) : error_(std::move(error_value)), has_value_(false) {}

  [[nodiscard]] bool has_value() const noexcept { return has_value_; }
  explicit operator bool() const noexcept { return has_value(); }
  [[nodiscard]] E& error() & { return error_; }
  [[nodiscard]] const E& error() const& { return error_; }

private:
  E error_{};
  bool has_value_{true};
};

namespace detail {
template <class T, class = void>
struct is_result : std::false_type {};

template <class T>
struct is_result<T, std::void_t<typename T::value_type, typename T::error_type>>
    : std::bool_constant<requires(T value) { { value.has_value() } -> std::convertible_to<bool>; }> {};
} // namespace detail

template <class T>
inline constexpr bool is_result_v = detail::is_result<std::remove_cvref_t<T>>::value;

} // namespace pb::runtime
