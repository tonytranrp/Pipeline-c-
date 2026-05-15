#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

#include <pb/runtime/error.hpp>
#include <pb/runtime/result.hpp>
#include <pb/runtime/sequential.hpp>

struct add_one {
    static constexpr auto name = "add_one";
    int operator()(int value) const { return value + 1; }
};

struct fail_expected {
    static constexpr auto name = "fail_expected";
    pb::runtime::result<int> operator()(int) const
    {
        return pb::runtime::error{.stage = {"stage-2", "fail_expected"},
                                  .category = pb::runtime::error_category::stage_failure,
                                  .message = "stopped"};
    }
};

struct never_run {
    static constexpr auto name = "never_run";
    int operator()(int value) const
    {
        assert(false && "later stage should not run after failure");
        return value;
    }
};

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

    auto failed = result<int>{error{.stage = {"stage-1", "parse"},
                                    .category = error_category::stage_failure,
                                    .message = "boom"}};
    assert(!failed.has_value());
    assert(failed.error().message == "boom");

    auto converted = to_result(fake_expected{.ok = false, .error_ = "bad"});
    assert(!converted.has_value());
    assert(converted.error().category == error_category::expected_error);

    auto built = make_result(13);
    assert(built.has_value());
    assert(built.value() == 13);

    auto engine = pb::runtime::make_sequential_engine(add_one{}, fail_expected{}, never_run{});
    auto chained = engine.run(41);
    assert(!chained.has_value());
    assert(chained.error().stage.name == std::string_view{"fail_expected"});
    assert(chained.error().message == "stopped");

    auto ok_engine = pb::runtime::make_sequential_engine(add_one{}, add_one{});
    auto run = ok_engine.run(1);
    assert(run.has_value());
    assert(run.value() == 3);

    return 0;
}

