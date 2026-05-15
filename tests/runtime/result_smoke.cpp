#include "pb/runtime/result.hpp"

#include <cassert>
#include <memory>
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
    assert(!ok.has_error());
    assert(ok.value() == 7);
    assert(ok.value_or(99) == 7);
    assert(ok.error_or(error{.message = "fallback"}).message == "fallback");

    auto failed = result<int>{error{.stage = {"stage-1", "parse"}, .category = error_category::stage_failure, .message = "boom"}};
    assert(!failed.has_value());
    assert(failed.has_error());
    assert(failed.value_or(99) == 99);
    assert(failed.error().message == "boom");
    assert(failed.error_or(error{.message = "fallback"}).message == "boom");

    auto void_ok = result<void>{};
    assert(void_ok.has_value());
    assert(!void_ok.has_error());
    assert(void_ok.error_or(error{.message = "fallback"}).message == "fallback");

    auto void_failed = result<void>{error{.message = "void boom"}};
    assert(!void_failed.has_value());
    assert(void_failed.has_error());
    assert(void_failed.error_or(error{.message = "fallback"}).message == "void boom");

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

    struct move_only_expected {
        using value_type = std::unique_ptr<int>;
        using error_type = std::string;
        bool ok{};
        std::unique_ptr<int> value_{};
        std::string error_{};

        bool has_value() const { return ok; }
        std::unique_ptr<int>& value() & { return value_; }
        std::unique_ptr<int>&& value() && { return std::move(value_); }
        std::string& error() & { return error_; }
        std::string&& error() && { return std::move(error_); }
    };

    auto moved_value = to_result(move_only_expected{.ok = true, .value_ = std::make_unique<int>(21)});
    assert(moved_value.has_value());
    assert(*moved_value.value() == 21);

    auto moved_error = to_result(move_only_expected{.ok = false, .error_ = "move-only bad"});
    assert(!moved_error.has_value());
    assert(moved_error.error().message == "move-only bad");

    struct void_expected {
        using value_type = void;
        using error_type = std::string;
        bool ok{};
        std::string error_{};

        bool has_value() const { return ok; }
        void value() const {}
        std::string const& error() const { return error_; }
    };

    auto converted_void_ok = to_result(void_expected{.ok = true});
    assert(converted_void_ok.has_value());
    assert(!converted_void_ok.has_error());

    auto converted_void_error = to_result(void_expected{.ok = false, .error_ = "void expected bad"});
    assert(!converted_void_error.has_value());
    assert(converted_void_error.error().category == error_category::expected_error);
    assert(converted_void_error.error().message == "void expected bad");

    auto built = make_result(13);
    assert(built.has_value());
    assert(built.value() == 13);

    auto built_void = make_result();
    assert(built_void.has_value());
    assert(!built_void.has_error());

    return 0;
}
