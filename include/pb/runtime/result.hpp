#pragma once

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

#include <pb/runtime/error.hpp>

namespace pb::runtime {

template <class E>
class unexpected;

template <class T, class E>
class expected;

template <class E>
class unexpected {
public:
    using error_type = E;

    constexpr explicit unexpected(const E& error) : error_(error) {}
    constexpr explicit unexpected(E&& error) : error_(std::move(error)) {}

    [[nodiscard]] constexpr auto error() const& -> const E& { return error_; }
    [[nodiscard]] constexpr auto error() & -> E& { return error_; }
    [[nodiscard]] constexpr auto error() && -> E&& { return std::move(error_); }

private:
    E error_;
};

template <class E>
[[nodiscard]] constexpr auto make_unexpected(E error) -> unexpected<E> {
    return unexpected<E>(std::move(error));
}

namespace detail {

template <class T>
struct is_expected : std::false_type {};

template <class T, class E>
struct is_expected<expected<T, E>> : std::true_type {};

template <class T>
inline constexpr bool is_expected_v = is_expected<std::remove_cvref_t<T>>::value;

template <class T>
struct expected_traits;

template <class T, class E>
struct expected_traits<expected<T, E>> {
    using value_type = T;
    using error_type = E;
};

template <class T>
using expected_value_t = typename expected_traits<std::remove_cvref_t<T>>::value_type;

template <class T>
using expected_error_t = typename expected_traits<std::remove_cvref_t<T>>::error_type;

}  // namespace detail

template <class T, class E>
class expected {
public:
    using value_type = T;
    using error_type = E;

    constexpr explicit expected(const T& value) : value_(value) {}
    constexpr explicit expected(T&& value) : value_(std::move(value)) {}
    constexpr explicit expected(const unexpected<E>& error) : error_(error.error()) {}
    constexpr explicit expected(unexpected<E>&& error) : error_(std::move(error).error()) {}
    constexpr explicit expected(const E& error) : error_(error) {}
    constexpr explicit expected(E&& error) : error_(std::move(error)) {}

    [[nodiscard]] constexpr auto has_value() const noexcept -> bool { return value_.has_value(); }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return has_value(); }

    [[nodiscard]] constexpr auto value() & -> T& { return *value_; }
    [[nodiscard]] constexpr auto value() const& -> const T& { return *value_; }
    [[nodiscard]] constexpr auto value() && -> T&& { return std::move(*value_); }
    [[nodiscard]] constexpr auto value() const&& -> const T&& { return std::move(*value_); }

    [[nodiscard]] constexpr auto error() & -> E& { return *error_; }
    [[nodiscard]] constexpr auto error() const& -> const E& { return *error_; }
    [[nodiscard]] constexpr auto error() && -> E&& { return std::move(*error_); }
    [[nodiscard]] constexpr auto error() const&& -> const E&& { return std::move(*error_); }

    template <class U>
    [[nodiscard]] constexpr auto value_or(U&& fallback) const& -> T {
        return has_value() ? *value_ : static_cast<T>(std::forward<U>(fallback));
    }

    template <class F>
    constexpr auto and_then(F&& f) & {
        using result_type = std::invoke_result_t<F, T&>;
        static_assert(detail::is_expected_v<result_type>, "and_then requires an expected-like return type");
        static_assert(std::is_same_v<detail::expected_error_t<result_type>, E>,
                      "and_then requires the same error type");
        if (!has_value()) {
            return result_type(unexpected<E>(error()));
        }
        return std::invoke(std::forward<F>(f), value());
    }

    template <class F>
    constexpr auto and_then(F&& f) const& {
        using result_type = std::invoke_result_t<F, const T&>;
        static_assert(detail::is_expected_v<result_type>, "and_then requires an expected-like return type");
        static_assert(std::is_same_v<detail::expected_error_t<result_type>, E>,
                      "and_then requires the same error type");
        if (!has_value()) {
            return result_type(unexpected<E>(error()));
        }
        return std::invoke(std::forward<F>(f), value());
    }

    template <class F>
    constexpr auto transform(F&& f) & {
        using result_type = std::invoke_result_t<F, T&>;
        static_assert(!detail::is_expected_v<result_type>, "transform requires a value-returning callable");
        if (!has_value()) {
            return expected<result_type, E>(unexpected<E>(error()));
        }
        if constexpr (std::is_void_v<result_type>) {
            std::invoke(std::forward<F>(f), value());
            return expected<void, E>{};
        } else {
            return expected<result_type, E>(std::invoke(std::forward<F>(f), value()));
        }
    }

    template <class F>
    constexpr auto transform(F&& f) const& {
        using result_type = std::invoke_result_t<F, const T&>;
        static_assert(!detail::is_expected_v<result_type>, "transform requires a value-returning callable");
        if (!has_value()) {
            return expected<result_type, E>(unexpected<E>(error()));
        }
        if constexpr (std::is_void_v<result_type>) {
            std::invoke(std::forward<F>(f), value());
            return expected<void, E>{};
        } else {
            return expected<result_type, E>(std::invoke(std::forward<F>(f), value()));
        }
    }

    template <class F>
    constexpr auto transform_error(F&& f) & {
        using mapped_error = std::invoke_result_t<F, E&>;
        static_assert(!std::is_void_v<mapped_error>, "transform_error requires a value-returning callable");
        if (has_value()) {
            return expected<T, mapped_error>(value());
        }
        return expected<T, mapped_error>(unexpected<mapped_error>(std::invoke(std::forward<F>(f), error())));
    }

    template <class F>
    constexpr auto transform_error(F&& f) const& {
        using mapped_error = std::invoke_result_t<F, const E&>;
        static_assert(!std::is_void_v<mapped_error>, "transform_error requires a value-returning callable");
        if (has_value()) {
            return expected<T, mapped_error>(value());
        }
        return expected<T, mapped_error>(unexpected<mapped_error>(std::invoke(std::forward<F>(f), error())));
    }

private:
    std::optional<T> value_{};
    std::optional<E> error_{};
};

template <class E>
class expected<void, E> {
public:
    using value_type = void;
    using error_type = E;

    constexpr expected() noexcept = default;
    constexpr explicit expected(const unexpected<E>& error) : has_value_{false}, error_(error.error()) {}
    constexpr explicit expected(unexpected<E>&& error) : has_value_{false}, error_(std::move(error).error()) {}
    constexpr explicit expected(const E& error) : has_value_{false}, error_(error) {}
    constexpr explicit expected(E&& error) : has_value_{false}, error_(std::move(error)) {}

    [[nodiscard]] constexpr auto has_value() const noexcept -> bool { return has_value_; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return has_value(); }

    constexpr auto value() const -> void {}

    [[nodiscard]] constexpr auto error() & -> E& { return *error_; }
    [[nodiscard]] constexpr auto error() const& -> const E& { return *error_; }
    [[nodiscard]] constexpr auto error() && -> E&& { return std::move(*error_); }
    [[nodiscard]] constexpr auto error() const&& -> const E&& { return std::move(*error_); }

    template <class F>
    constexpr auto and_then(F&& f) & {
        using result_type = std::invoke_result_t<F>;
        static_assert(detail::is_expected_v<result_type>, "and_then requires an expected-like return type");
        static_assert(std::is_same_v<detail::expected_error_t<result_type>, E>,
                      "and_then requires the same error type");
        if (!has_value()) {
            return result_type(unexpected<E>(error()));
        }
        return std::invoke(std::forward<F>(f));
    }

    template <class F>
    constexpr auto and_then(F&& f) const& {
        using result_type = std::invoke_result_t<F>;
        static_assert(detail::is_expected_v<result_type>, "and_then requires an expected-like return type");
        static_assert(std::is_same_v<detail::expected_error_t<result_type>, E>,
                      "and_then requires the same error type");
        if (!has_value()) {
            return result_type(unexpected<E>(error()));
        }
        return std::invoke(std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) & {
        using result_type = std::invoke_result_t<F>;
        static_assert(!detail::is_expected_v<result_type>, "transform requires a value-returning callable");
        if (!has_value()) {
            return expected<result_type, E>(unexpected<E>(error()));
        }
        if constexpr (std::is_void_v<result_type>) {
            std::invoke(std::forward<F>(f));
            return expected<void, E>{};
        } else {
            return expected<result_type, E>(std::invoke(std::forward<F>(f)));
        }
    }

    template <class F>
    constexpr auto transform(F&& f) const& {
        using result_type = std::invoke_result_t<F>;
        static_assert(!detail::is_expected_v<result_type>, "transform requires a value-returning callable");
        if (!has_value()) {
            return expected<result_type, E>(unexpected<E>(error()));
        }
        if constexpr (std::is_void_v<result_type>) {
            std::invoke(std::forward<F>(f));
            return expected<void, E>{};
        } else {
            return expected<result_type, E>(std::invoke(std::forward<F>(f)));
        }
    }

    template <class F>
    constexpr auto transform_error(F&& f) & {
        using mapped_error = std::invoke_result_t<F, E&>;
        static_assert(!std::is_void_v<mapped_error>, "transform_error requires a value-returning callable");
        if (has_value()) {
            return expected<void, mapped_error>{};
        }
        return expected<void, mapped_error>(unexpected<mapped_error>(std::invoke(std::forward<F>(f), error())));
    }

    template <class F>
    constexpr auto transform_error(F&& f) const& {
        using mapped_error = std::invoke_result_t<F, const E&>;
        static_assert(!std::is_void_v<mapped_error>, "transform_error requires a value-returning callable");
        if (has_value()) {
            return expected<void, mapped_error>{};
        }
        return expected<void, mapped_error>(unexpected<mapped_error>(std::invoke(std::forward<F>(f), error())));
    }

private:
    bool has_value_{true};
    std::optional<E> error_{};
};

template <class T>
using result = expected<T, error>;

template <class T>
using result_value_t = typename result<T>::value_type;

template <class T>
using result_error_t = typename result<T>::error_type;

template <class T>
inline constexpr bool is_result_v = detail::is_expected_v<T> &&
                                    std::is_same_v<detail::expected_error_t<T>, error>;

}  // namespace pb::runtime
