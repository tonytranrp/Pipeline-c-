#include "pb/runtime/sequential.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <string_view>

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

struct throwing_stage {
    static constexpr auto name = "throwing_stage";
    int operator()(int) const
    {
        throw std::runtime_error{"explode"};
    }
};

int main()
{
    auto engine = pb::runtime::make_sequential_engine(add_one{}, fail_expected{}, never_run{});
    auto failed = engine.run(41);
    assert(!failed.has_value());
    assert(failed.error().stage.name == std::string_view{"fail_expected"});
    assert(failed.error().message == "stopped");

    auto ok_engine = pb::runtime::make_sequential_engine(add_one{}, add_one{});
    auto ok = ok_engine.run(1);
    assert(ok.has_value());
    assert(ok.value() == 3);

    auto throw_engine = pb::runtime::make_sequential_engine(throwing_stage{});
    auto thrown = throw_engine.run(5);
    assert(!thrown.has_value());
    assert(thrown.error().category == pb::runtime::error_category::exception);
    assert(thrown.error().message == "explode");

    return 0;
}
