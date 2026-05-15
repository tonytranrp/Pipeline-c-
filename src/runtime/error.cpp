#include <pb/runtime/error.hpp>

#include <ostream>
#include <sstream>

namespace pb::runtime {

auto error::detail(std::string_view key) const -> const std::string*
{
    for (auto const& entry : details) {
        if (entry.key == key) {
            return &entry.value;
        }
    }
    return nullptr;
}

auto make_error(error_code code, std::string message, stage_id stage, std::source_location location) -> error
{
    return error{code, std::move(message), stage, location};
}

auto to_string(error_code code) noexcept -> std::string_view
{
    switch (code) {
    case error_code::none:
        return "none";
    case error_code::invalid_input:
        return "invalid_input";
    case error_code::stage_failure:
        return "stage_failure";
    case error_code::expected_error:
        return "expected_error";
    case error_code::exception:
        return "exception";
    case error_code::cancelled:
        return "cancelled";
    case error_code::timeout:
        return "timeout";
    case error_code::backend_failure:
        return "backend_failure";
    case error_code::invalid_pipeline:
        return "invalid_pipeline";
    case error_code::empty_pipeline:
        return "empty_pipeline";
    case error_code::unknown:
        return "unknown";
    }
    return "unknown";
}

auto serialize(error const& value) -> std::string
{
    std::ostringstream out;
    out << "{"
        << "\"stage_id\":\"" << value.stage.id << "\","
        << "\"stage_name\":\"" << value.stage.name << "\","
        << "\"stage_index\":" << value.stage.index << ","
        << "\"code\":\"" << to_string(value.effective_code()) << "\","
        << "\"message\":\"" << value.message << "\","
        << "\"file\":\"" << value.location.file_name() << "\","
        << "\"line\":" << value.location.line()
        << "}";
    return out.str();
}

auto operator<<(std::ostream& out, error const& value) -> std::ostream&
{
    return out << serialize(value);
}

} // namespace pb::runtime
