#include "pb/runtime/result.hpp"

#include <cassert>
#include <string>

int main()
{
    using pb::runtime::error;
    using pb::runtime::error_category;
    using pb::runtime::make_result;
    using pb::runtime::result;
    using pb::runtime::to_result;

    auto ok = result<int>{7};
    assert(ok.has_value());
    assert(ok.value() == 7);

    auto failed = result<int>{error{.stage = {"stage-1", "parse"}, .category = error_category::stage_failure, .message = "boom"}};
    assert(!failed.has_value());
    assert(failed.error().message == "boom");

    struct fake_expected {
        using value_type = int;
        using error_type = std::string;
        bool ok{};
        int value_{};
        std::string error_{};

        bool has_value() const { return ok; }
        int value() const { return value_; }
        std::string const& error() const { return error_; }
    };

    auto converted = to_result(fake_expected{.ok = false, .error_ = "bad"});
    assert(!converted.has_value());
    assert(converted.error().category == error_category::expected_error);

    auto built = make_result(13);
    assert(built.has_value());
    assert(built.value() == 13);

    return 0;
}

