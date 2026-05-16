#include <pb/pipeline.hpp>

#include <cassert>
#include <string>

struct Input {
  int value{};
};

struct Output {
  int value{};
};

struct custom_error {
  pb::runtime::error diagnostic{};

  custom_error() = default;
  explicit custom_error(std::string message)
      : diagnostic{.category = pb::runtime::error_category::stage_failure, .message = std::move(message)} {}
  custom_error(pb::runtime::error value) : diagnostic(std::move(value)) {}
};

struct CheckedWithDiagnostic {
  using input_type = Input;
  using output_type = Output;
  using error_type = custom_error;

  static constexpr auto stage_name() noexcept { return "checked_with_diagnostic"; }

  pb::runtime::result<Output, custom_error> operator()(Input input) const {
    if (input.value < 0) {
      return custom_error{"custom failure"};
    }
    return Output{input.value + 1};
  }
};

using Pipeline = pb::from<Input>::then<CheckedWithDiagnostic>::to<Output>;

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto ok = engine.try_run(Input{20});
  assert(ok.has_value());
  assert(ok.value().value == 21);

  auto failed = engine.try_run(Input{-1});
  assert(!failed.has_value());
  assert(failed.error().category == pb::runtime::error_category::expected_error);
  assert(failed.error().message == "custom failure");

  return 0;
}
