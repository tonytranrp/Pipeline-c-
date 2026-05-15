#pragma once

#include <cstddef>
#include <cstdint>
#include <source_location>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace pb::runtime {

using stage_id = std::size_t;

inline constexpr stage_id npos = static_cast<stage_id>(-1);

enum class error_code : std::uint8_t {
    none = 0,
    invalid_input,
    stage_failure,
    cancelled,
    timeout,
    exception,
    backend_failure,
    unknown,
};

struct error_detail {
    std::string key;
    std::string value;
};

using metadata = std::vector<error_detail>;

struct error {
    error_code code{error_code::none};
    std::string message{};
    stage_id stage{npos};
    std::source_location location{std::source_location::current()};
    metadata details{};

    constexpr error() noexcept = default;

    constexpr error(error_code code_,
                    std::string message_ = {},
                    stage_id stage_ = npos,
                    std::source_location location_ = std::source_location::current())
        : code(code_), message(std::move(message_)), stage(stage_), location(location_) {}

    [[nodiscard]] constexpr auto ok() const noexcept -> bool { return code == error_code::none; }
    [[nodiscard]] constexpr auto failed() const noexcept -> bool { return !ok(); }

    auto with_detail(std::string key, std::string value) -> error& {
        details.push_back(error_detail{std::move(key), std::move(value)});
        return *this;
    }

    [[nodiscard]] auto detail(std::string_view key) const -> const std::string* {
        for (const auto& entry : details) {
            if (entry.key == key) {
                return &entry.value;
            }
        }
        return nullptr;
    }
};

[[nodiscard]] inline auto make_error(error_code code,
                                     std::string message = {},
                                     stage_id stage = npos,
                                     std::source_location location = std::source_location::current()) -> error {
    return error{code, std::move(message), stage, location};
}

}  // namespace pb::runtime
