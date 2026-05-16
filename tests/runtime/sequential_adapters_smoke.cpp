#include <pb/pipeline.hpp>

#include <cassert>
#include <exception>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

struct Input {
  int value{};
};

struct Parsed {
  int value{};
};

struct MoveOnlyParsed {
  std::unique_ptr<int> value{};
};

struct Output {
  int value{};
};

struct MoveOnlyOutput {
  std::unique_ptr<int> value{};
};

struct opaque_error {
  int code{};
};

struct diagnostic_error {
  pb::runtime::error diagnostic{};
};

struct move_only_diagnostic_error {
  pb::runtime::error diagnostic{};
  std::unique_ptr<int> token{};
};

struct move_only_opaque_error {
  std::unique_ptr<int> code{};
};

template <class T, class E>
struct external_expected {
  using value_type = T;
  using error_type = E;

  bool ok{};
  T value_{};
  E error_{};

  [[nodiscard]] bool has_value() const { return ok; }
  [[nodiscard]] const T& value() const& { return value_; }
  [[nodiscard]] T&& value() && { return std::move(value_); }
  [[nodiscard]] const E& error() const& { return error_; }
  [[nodiscard]] E&& error() && { return std::move(error_); }
};

template <class E>
struct external_void_expected {
  using value_type = void;
  using error_type = E;

  bool ok{};
  E error_{};

  [[nodiscard]] bool has_value() const { return ok; }
  void value() const {}
  [[nodiscard]] const E& error() const& { return error_; }
  [[nodiscard]] E&& error() && { return std::move(error_); }
};

namespace adapter_stage_names {
struct parse_input {
  static constexpr auto value = "parse_input";
};

struct multiply_input {
  static constexpr auto value = "multiply_input";
};

struct parse_member {
  static constexpr auto value = "parse_member";
};

struct parse_functor {
  static constexpr auto value = "parse_functor";
};

struct parse_functor_move_only_diagnostic {
  static constexpr auto value = "parse_functor_move_only_diagnostic";
};

struct parse_opaque {
  static constexpr auto value = "parse_opaque";
};

struct parse_diagnostic {
  static constexpr auto value = "parse_diagnostic";
};

struct parse_member_move_only_diagnostic {
  static constexpr auto value = "parse_member_move_only_diagnostic";
};

struct parse_member_move_only_value {
  static constexpr auto value = "parse_member_move_only_value";
};

struct consume_void {
  static constexpr auto value = "consume_void";
};

struct consume_void_move_only {
  static constexpr auto value = "consume_void_move_only";
};

struct direct_member_consume_void {
  static constexpr auto value = "direct_member_consume_void";
};

struct direct_member_emit_move_only {
  static constexpr auto value = "direct_member_emit_move_only";
};
} // namespace adapter_stage_names

Parsed parse_input_fn(Input input) {
  return Parsed{input.value + 1};
}

Parsed parse_input_throwing(Input input) {
  if (input.value < -100) {
    throw std::runtime_error{"parse_input_throwing failed"};
  }
  return Parsed{input.value + 2};
}

Parsed double_input(Parsed parsed) {
  return Parsed{parsed.value * 2};
}

struct MemberParser {
  Parsed parse(Input input) const { return Parsed{input.value + 3}; }

  external_expected<Parsed, std::string> parse_checked(Input input) const {
    if (input.value < 0) {
      return {.ok = false, .error_ = "member parse failed"};
    }
    return {.ok = true, .value_ = {input.value + 3}};
  }

  external_expected<Parsed, move_only_diagnostic_error> parse_move_only_diagnostic(Input input) const {
    if (input.value < 0) {
      return {.ok = false,
              .error_ = {.diagnostic = {.stage = {.key = "member.external", .name = "MemberExternal"},
                                        .category = pb::runtime::error_category::stage_failure,
                                        .message = "move-only member diagnostic failed"},
                         .token = std::make_unique<int>(17)}};
    }
    return {.ok = true, .value_ = {input.value + 11}};
  }

  external_expected<MoveOnlyParsed, std::string> parse_move_only_value(Input input) const {
    if (input.value < 0) {
      return {.ok = false, .error_ = "member move-only value failed"};
    }
    return {.ok = true, .value_ = {.value = std::make_unique<int>(input.value + 17)}};
  }

  external_void_expected<std::string> consume_checked(Input input) const {
    if (input.value < 0) {
      return {.ok = false, .error_ = "member consume failed"};
    }
    return {.ok = true};
  }
};

struct MemberEmitter {
  Output emit(Parsed parsed) const { return Output{parsed.value + 4}; }

  MoveOnlyOutput emit_move_only(MoveOnlyParsed parsed) const {
    return MoveOnlyOutput{.value = std::make_unique<int>(*parsed.value + 4)};
  }
};

struct FunctorParser {
  external_expected<Parsed, std::string> operator()(Input input) const {
    if (input.value < 0) {
      return {.ok = false, .error_ = "functor parse failed"};
    }
    return {.ok = true, .value_ = {input.value + 5}};
  }
};

struct FunctorMoveOnlyDiagnosticParser {
  external_expected<Parsed, move_only_diagnostic_error> operator()(Input input) const {
    if (input.value < 0) {
      return {.ok = false,
              .error_ = {.diagnostic = {.stage = {.key = "functor.external", .name = "FunctorExternal"},
                                        .category = pb::runtime::error_category::stage_failure,
                                        .message = "move-only functor diagnostic failed"},
                         .token = std::make_unique<int>(23)}};
    }
    return {.ok = true, .value_ = {input.value + 13}};
  }
};

external_expected<Parsed, opaque_error> parse_opaque_error(Input input) {
  if (input.value < 0) {
    return {.ok = false, .error_ = {.code = 42}};
  }
  return {.ok = true, .value_ = {input.value + 7}};
}

external_expected<Parsed, diagnostic_error> parse_diagnostic_error(Input input) {
  if (input.value < 0) {
    return {.ok = false,
            .error_ = {.diagnostic = {.stage = {.key = "external.parse", .name = "ExternalParse"},
                                      .category = pb::runtime::error_category::stage_failure,
                                      .message = "diagnostic parse failed"}}};
  }
  return {.ok = true, .value_ = {input.value + 9}};
}

external_void_expected<opaque_error> consume_void_expected(Input input) {
  if (input.value < 0) {
    return {.ok = false, .error_ = {.code = 7}};
  }
  return {.ok = true};
}

external_void_expected<move_only_opaque_error> consume_void_move_only_expected(Input input) {
  if (input.value < 0) {
    return {.ok = false, .error_ = {.code = std::make_unique<int>(13)}};
  }
  return {.ok = true};
}

using ParsedAdapter = pb::adapt<pb::name<adapter_stage_names::parse_input>, pb::fn<parse_input_fn>,
                                 pb::in<Input>, pb::out<Parsed>>;
using ThrowingParsedAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_input>, pb::fn<parse_input_throwing>, pb::in<Input>,
             pb::out<Parsed>>;
using MultiplierAdapter =
    pb::adapt<pb::name<adapter_stage_names::multiply_input>, pb::fn<double_input>,
             pb::in<Parsed>, pb::out<Parsed>>;
using NamedDirectMemberAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_member>, pb::member<&MemberParser::parse>, pb::in<Input>,
              pb::out<Parsed>>;
using NamedDirectExpectedMemberAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_member>, pb::member<&MemberParser::parse_checked>, pb::in<Input>,
              pb::out<Parsed>>;
using DirectMemberMoveOnlyDiagnosticAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_member_move_only_diagnostic>,
              pb::member<&MemberParser::parse_move_only_diagnostic>, pb::in<Input>, pb::out<Parsed>>;
using DirectMemberMoveOnlyValueAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_member_move_only_value>,
              pb::member<&MemberParser::parse_move_only_value>, pb::in<Input>, pb::out<MoveOnlyParsed>>;
using UnnamedDirectMemberAdapter =
    pb::adapt<pb::member<&MemberEmitter::emit>, pb::in<Parsed>, pb::out<Output>>;
using DirectMemberMoveOnlyEmitterAdapter =
    pb::adapt<pb::name<adapter_stage_names::direct_member_emit_move_only>,
              pb::member<&MemberEmitter::emit_move_only>, pb::in<MoveOnlyParsed>, pb::out<MoveOnlyOutput>>;
using ExpectedFunctorAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_functor>, pb::functor<FunctorParser>, pb::in<Input>,
              pb::out<Parsed>>;
using FunctorMoveOnlyDiagnosticAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_functor_move_only_diagnostic>,
              pb::functor<FunctorMoveOnlyDiagnosticParser>, pb::in<Input>, pb::out<Parsed>>;
using OpaqueErrorAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_opaque>, pb::fn<parse_opaque_error>, pb::in<Input>,
              pb::out<Parsed>>;
using DiagnosticErrorAdapter =
    pb::adapt<pb::name<adapter_stage_names::parse_diagnostic>, pb::fn<parse_diagnostic_error>, pb::in<Input>,
              pb::out<Parsed>>;
using VoidExpectedAdapter =
    pb::adapt<pb::name<adapter_stage_names::consume_void>, pb::fn<consume_void_expected>, pb::in<Input>,
              pb::out<void>>;
using VoidMoveOnlyExpectedAdapter =
    pb::adapt<pb::name<adapter_stage_names::consume_void_move_only>, pb::fn<consume_void_move_only_expected>,
              pb::in<Input>, pb::out<void>>;
using DirectMemberVoidExpectedAdapter =
    pb::adapt<pb::name<adapter_stage_names::direct_member_consume_void>, pb::member<&MemberParser::consume_checked>,
              pb::in<Input>, pb::out<void>>;

struct Emit {
  using input_type = Parsed;
  using output_type = Output;
  using error_type = pb::runtime::error;

  static constexpr auto stage_key() noexcept { return "emit"; }
  static constexpr auto stage_name() noexcept { return "emit_result"; }

  pb::runtime::result<Output> operator()(Parsed parsed) const {
    if (parsed.value < 0) {
      return pb::runtime::error{.stage = {.key = "emit", .name = "emit_result"},
                               .category = pb::runtime::error_category::stage_failure,
                               .message = "invalid parsed value"};
    }
    return Output{parsed.value + 1};
  }
};

using Pipeline = pb::from<Input>::then<ParsedAdapter>::then<MultiplierAdapter>::then<Emit>::to<Output>;
using ThrowingPipeline =
    pb::from<Input>::then<ThrowingParsedAdapter>::then<MultiplierAdapter>::then<Emit>::to<Output>;
using ThrowingPipelineRaw = pb::from<Input>::then<ThrowingParsedAdapter>::then<MultiplierAdapter>::to<Parsed>;
using DirectMemberPipeline =
    pb::from<Input>::then<NamedDirectMemberAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using DirectExpectedMemberPipeline =
    pb::from<Input>::then<NamedDirectExpectedMemberAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using DirectMemberMoveOnlyDiagnosticPipeline =
    pb::from<Input>::then<DirectMemberMoveOnlyDiagnosticAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using DirectMemberMoveOnlyValuePipeline =
    pb::from<Input>::then<DirectMemberMoveOnlyValueAdapter>::to<MoveOnlyParsed>;
using DirectMemberMoveOnlyValueHandoffPipeline =
    pb::from<Input>::then<DirectMemberMoveOnlyValueAdapter>::then<DirectMemberMoveOnlyEmitterAdapter>::to<MoveOnlyOutput>;
using ExpectedFunctorPipeline =
    pb::from<Input>::then<ExpectedFunctorAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using FunctorMoveOnlyDiagnosticPipeline =
    pb::from<Input>::then<FunctorMoveOnlyDiagnosticAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using OpaqueErrorPipeline =
    pb::from<Input>::then<OpaqueErrorAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using DiagnosticErrorPipeline =
    pb::from<Input>::then<DiagnosticErrorAdapter>::then<UnnamedDirectMemberAdapter>::to<Output>;
using VoidExpectedPipeline = pb::from<Input>::then<VoidExpectedAdapter>::to<void>;
using VoidMoveOnlyExpectedPipeline = pb::from<Input>::then<VoidMoveOnlyExpectedAdapter>::to<void>;
using DirectMemberVoidExpectedPipeline = pb::from<Input>::then<DirectMemberVoidExpectedAdapter>::to<void>;

static_assert(pb::adapted_stage<ParsedAdapter>);
static_assert(pb::adapted_stage<MultiplierAdapter>);
static_assert(pb::adapted_stage<NamedDirectMemberAdapter>);
static_assert(pb::adapted_stage<NamedDirectExpectedMemberAdapter>);
static_assert(pb::adapted_stage<DirectMemberMoveOnlyDiagnosticAdapter>);
static_assert(pb::adapted_stage<DirectMemberMoveOnlyValueAdapter>);
static_assert(pb::adapted_stage<UnnamedDirectMemberAdapter>);
static_assert(pb::adapted_stage<DirectMemberMoveOnlyEmitterAdapter>);
static_assert(pb::adapted_stage<ExpectedFunctorAdapter>);
static_assert(pb::adapted_stage<FunctorMoveOnlyDiagnosticAdapter>);
static_assert(pb::adapted_stage<OpaqueErrorAdapter>);
static_assert(pb::adapted_stage<DiagnosticErrorAdapter>);
static_assert(pb::adapted_stage<VoidExpectedAdapter>);
static_assert(pb::adapted_stage<VoidMoveOnlyExpectedAdapter>);
static_assert(pb::adapted_stage<DirectMemberVoidExpectedAdapter>);
static_assert(pb::valid<Pipeline>);
static_assert(pb::valid<ThrowingPipeline>);
static_assert(pb::valid<ThrowingPipelineRaw>);
static_assert(pb::valid<DirectMemberPipeline>);
static_assert(pb::valid<DirectExpectedMemberPipeline>);
static_assert(pb::valid<DirectMemberMoveOnlyDiagnosticPipeline>);
static_assert(pb::valid<DirectMemberMoveOnlyValuePipeline>);
static_assert(pb::valid<DirectMemberMoveOnlyValueHandoffPipeline>);
static_assert(pb::valid<ExpectedFunctorPipeline>);
static_assert(pb::valid<FunctorMoveOnlyDiagnosticPipeline>);
static_assert(pb::valid<OpaqueErrorPipeline>);
static_assert(pb::valid<DiagnosticErrorPipeline>);
static_assert(pb::valid<VoidExpectedPipeline>);
static_assert(pb::valid<VoidMoveOnlyExpectedPipeline>);
static_assert(pb::valid<DirectMemberVoidExpectedPipeline>);

struct recording_observer final : pb::runtime::observer {
  std::vector<std::string> events{};

  void on_stage_start(const pb::runtime::stage_id& stage) override {
    events.push_back("start:" + stage.name + "/" + stage.key);
  }

  void on_stage_exception(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("exception:" + stage.name + "/" + stage.key + ":" + error.message);
  }

  void on_stage_success(const pb::runtime::stage_id& stage) override {
    events.push_back("success:" + stage.name + "/" + stage.key);
  }

  void on_stage_failure(const pb::runtime::stage_id& stage, const pb::runtime::error& error) override {
    events.push_back("failure:" + stage.name + "/" + stage.key + ":" + pb::runtime::describe(error));
  }
};

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});

  auto ok = engine.run(Input{4});
  assert(ok.has_value());
  assert(ok.value().value == 11);

  auto direct_member_engine = pb::compile<DirectMemberPipeline>(pb::runtime::sequential{});
  auto direct_member_output = direct_member_engine.run(Input{5});
  assert(direct_member_output.value == 12);

  auto direct_expected_member_engine = pb::compile<DirectExpectedMemberPipeline>(pb::runtime::sequential{});
  recording_observer expected_observer{};
  direct_expected_member_engine.set_observer(&expected_observer);

  auto direct_expected_failed = direct_expected_member_engine.try_run(Input{-5});
  assert(!direct_expected_failed.has_value());
  assert(direct_expected_failed.error().category == pb::runtime::error_category::expected_error);
  assert(direct_expected_failed.error().stage.key == "parse_member");
  assert(direct_expected_failed.error().stage.name == "parse_member");
  assert(direct_expected_failed.error().message == "member parse failed");
  assert(pb::runtime::describe(direct_expected_failed.error()) ==
         "expected_error at parse_member: member parse failed");
  assert((expected_observer.events == std::vector<std::string>{
                                          "start:parse_member/parse_member",
                                          "failure:parse_member/parse_member:expected_error at parse_member: "
                                          "member parse failed",
                                      }));

  auto direct_member_move_only_diagnostic_engine =
      pb::compile<DirectMemberMoveOnlyDiagnosticPipeline>(pb::runtime::sequential{});
  recording_observer direct_member_move_only_diagnostic_observer{};
  direct_member_move_only_diagnostic_engine.set_observer(&direct_member_move_only_diagnostic_observer);

  auto direct_member_move_only_diagnostic_failed =
      direct_member_move_only_diagnostic_engine.try_run(Input{-5});
  assert(!direct_member_move_only_diagnostic_failed.has_value());
  assert(direct_member_move_only_diagnostic_failed.error().category ==
         pb::runtime::error_category::expected_error);
  assert(direct_member_move_only_diagnostic_failed.error().stage.key ==
         "parse_member_move_only_diagnostic");
  assert(direct_member_move_only_diagnostic_failed.error().stage.name ==
         "parse_member_move_only_diagnostic");
  assert(direct_member_move_only_diagnostic_failed.error().message == "move-only member diagnostic failed");
  assert(pb::runtime::describe(direct_member_move_only_diagnostic_failed.error()) ==
         "expected_error at parse_member_move_only_diagnostic: move-only member diagnostic failed");
  assert((direct_member_move_only_diagnostic_observer.events ==
          std::vector<std::string>{
              "start:parse_member_move_only_diagnostic/parse_member_move_only_diagnostic",
              "failure:parse_member_move_only_diagnostic/parse_member_move_only_diagnostic:"
              "expected_error at parse_member_move_only_diagnostic: move-only member diagnostic failed",
          }));

  auto direct_member_move_only_diagnostic_raw_failed =
      direct_member_move_only_diagnostic_engine.run(Input{-5});
  assert(!direct_member_move_only_diagnostic_raw_failed.has_value());
  assert(direct_member_move_only_diagnostic_raw_failed.error().category ==
         pb::runtime::error_category::expected_error);
  assert(direct_member_move_only_diagnostic_raw_failed.error().stage.key ==
         "parse_member_move_only_diagnostic");
  assert(direct_member_move_only_diagnostic_raw_failed.error().stage.name ==
         "parse_member_move_only_diagnostic");
  assert(direct_member_move_only_diagnostic_raw_failed.error().message ==
         "move-only member diagnostic failed");

  auto direct_member_move_only_value_engine = pb::compile<DirectMemberMoveOnlyValuePipeline>(pb::runtime::sequential{});
  recording_observer direct_member_move_only_value_observer{};
  direct_member_move_only_value_engine.set_observer(&direct_member_move_only_value_observer);

  auto direct_member_move_only_value_ok = direct_member_move_only_value_engine.try_run(Input{5});
  assert(direct_member_move_only_value_ok.has_value());
  assert(!direct_member_move_only_value_ok.has_error());
  assert(direct_member_move_only_value_ok.value().value != nullptr);
  assert(*direct_member_move_only_value_ok.value().value == 22);

  auto direct_member_move_only_value_failed = direct_member_move_only_value_engine.try_run(Input{-5});
  assert(!direct_member_move_only_value_failed.has_value());
  assert(direct_member_move_only_value_failed.error().category == pb::runtime::error_category::expected_error);
  assert(direct_member_move_only_value_failed.error().stage.key == "parse_member_move_only_value");
  assert(direct_member_move_only_value_failed.error().stage.name == "parse_member_move_only_value");
  assert(direct_member_move_only_value_failed.error().message == "member move-only value failed");
  assert(pb::runtime::describe(direct_member_move_only_value_failed.error()) ==
         "expected_error at parse_member_move_only_value: member move-only value failed");
  assert((direct_member_move_only_value_observer.events ==
          std::vector<std::string>{
              "start:parse_member_move_only_value/parse_member_move_only_value",
              "success:parse_member_move_only_value/parse_member_move_only_value",
              "start:parse_member_move_only_value/parse_member_move_only_value",
              "failure:parse_member_move_only_value/parse_member_move_only_value:"
              "expected_error at parse_member_move_only_value: member move-only value failed",
          }));

  auto direct_member_move_only_value_raw_ok = direct_member_move_only_value_engine.run(Input{7});
  assert(direct_member_move_only_value_raw_ok.has_value());
  assert(!direct_member_move_only_value_raw_ok.has_error());
  assert(direct_member_move_only_value_raw_ok.value().value != nullptr);
  assert(*direct_member_move_only_value_raw_ok.value().value == 24);

  auto direct_member_move_only_handoff_engine =
      pb::compile<DirectMemberMoveOnlyValueHandoffPipeline>(pb::runtime::sequential{});
  recording_observer direct_member_move_only_handoff_observer{};
  direct_member_move_only_handoff_engine.set_observer(&direct_member_move_only_handoff_observer);

  auto direct_member_move_only_handoff_ok = direct_member_move_only_handoff_engine.try_run(Input{5});
  assert(direct_member_move_only_handoff_ok.has_value());
  assert(!direct_member_move_only_handoff_ok.has_error());
  assert(direct_member_move_only_handoff_ok.value().value != nullptr);
  assert(*direct_member_move_only_handoff_ok.value().value == 26);

  auto direct_member_move_only_handoff_failed = direct_member_move_only_handoff_engine.try_run(Input{-5});
  assert(!direct_member_move_only_handoff_failed.has_value());
  assert(direct_member_move_only_handoff_failed.error().category == pb::runtime::error_category::expected_error);
  assert(direct_member_move_only_handoff_failed.error().stage.key == "parse_member_move_only_value");
  assert(direct_member_move_only_handoff_failed.error().stage.name == "parse_member_move_only_value");
  assert(direct_member_move_only_handoff_failed.error().message == "member move-only value failed");
  assert((direct_member_move_only_handoff_observer.events ==
          std::vector<std::string>{
              "start:parse_member_move_only_value/parse_member_move_only_value",
              "success:parse_member_move_only_value/parse_member_move_only_value",
              "start:direct_member_emit_move_only/direct_member_emit_move_only",
              "success:direct_member_emit_move_only/direct_member_emit_move_only",
              "start:parse_member_move_only_value/parse_member_move_only_value",
              "failure:parse_member_move_only_value/parse_member_move_only_value:"
              "expected_error at parse_member_move_only_value: member move-only value failed",
          }));

  auto direct_member_move_only_handoff_raw_ok = direct_member_move_only_handoff_engine.run(Input{7});
  assert(direct_member_move_only_handoff_raw_ok.has_value());
  assert(!direct_member_move_only_handoff_raw_ok.has_error());
  assert(direct_member_move_only_handoff_raw_ok.value().value != nullptr);
  assert(*direct_member_move_only_handoff_raw_ok.value().value == 28);

  auto expected_functor_engine = pb::compile<ExpectedFunctorPipeline>(pb::runtime::sequential{});
  recording_observer functor_observer{};
  expected_functor_engine.set_observer(&functor_observer);

  auto expected_functor_failed = expected_functor_engine.try_run(Input{-5});
  assert(!expected_functor_failed.has_value());
  assert(expected_functor_failed.error().category == pb::runtime::error_category::expected_error);
  assert(expected_functor_failed.error().stage.key == "parse_functor");
  assert(expected_functor_failed.error().stage.name == "parse_functor");
  assert(expected_functor_failed.error().message == "functor parse failed");
  assert(pb::runtime::describe(expected_functor_failed.error()) ==
         "expected_error at parse_functor: functor parse failed");
  assert((functor_observer.events == std::vector<std::string>{
                                         "start:parse_functor/parse_functor",
                                         "failure:parse_functor/parse_functor:expected_error at parse_functor: "
                                         "functor parse failed",
                                     }));

  auto functor_move_only_diagnostic_engine =
      pb::compile<FunctorMoveOnlyDiagnosticPipeline>(pb::runtime::sequential{});
  recording_observer functor_move_only_diagnostic_observer{};
  functor_move_only_diagnostic_engine.set_observer(&functor_move_only_diagnostic_observer);

  auto functor_move_only_diagnostic_failed = functor_move_only_diagnostic_engine.try_run(Input{-5});
  assert(!functor_move_only_diagnostic_failed.has_value());
  assert(functor_move_only_diagnostic_failed.error().category == pb::runtime::error_category::expected_error);
  assert(functor_move_only_diagnostic_failed.error().stage.key == "parse_functor_move_only_diagnostic");
  assert(functor_move_only_diagnostic_failed.error().stage.name == "parse_functor_move_only_diagnostic");
  assert(functor_move_only_diagnostic_failed.error().message == "move-only functor diagnostic failed");
  assert(pb::runtime::describe(functor_move_only_diagnostic_failed.error()) ==
         "expected_error at parse_functor_move_only_diagnostic: move-only functor diagnostic failed");
  assert((functor_move_only_diagnostic_observer.events ==
          std::vector<std::string>{
              "start:parse_functor_move_only_diagnostic/parse_functor_move_only_diagnostic",
              "failure:parse_functor_move_only_diagnostic/parse_functor_move_only_diagnostic:"
              "expected_error at parse_functor_move_only_diagnostic: move-only functor diagnostic failed",
          }));

  auto functor_move_only_diagnostic_raw_failed = functor_move_only_diagnostic_engine.run(Input{-5});
  assert(!functor_move_only_diagnostic_raw_failed.has_value());
  assert(functor_move_only_diagnostic_raw_failed.error().category ==
         pb::runtime::error_category::expected_error);
  assert(functor_move_only_diagnostic_raw_failed.error().stage.key ==
         "parse_functor_move_only_diagnostic");
  assert(functor_move_only_diagnostic_raw_failed.error().stage.name ==
         "parse_functor_move_only_diagnostic");
  assert(functor_move_only_diagnostic_raw_failed.error().message ==
         "move-only functor diagnostic failed");

  auto opaque_error_engine = pb::compile<OpaqueErrorPipeline>(pb::runtime::sequential{});
  recording_observer opaque_observer{};
  opaque_error_engine.set_observer(&opaque_observer);

  auto opaque_failed = opaque_error_engine.try_run(Input{-5});
  assert(!opaque_failed.has_value());
  assert(opaque_failed.error().category == pb::runtime::error_category::expected_error);
  assert(opaque_failed.error().stage.key == "parse_opaque");
  assert(opaque_failed.error().stage.name == "parse_opaque");
  assert(opaque_failed.error().message == "expected-like object reported an error");
  assert(pb::runtime::describe(opaque_failed.error()) ==
         "expected_error at parse_opaque: expected-like object reported an error");
  assert((opaque_observer.events == std::vector<std::string>{
                                           "start:parse_opaque/parse_opaque",
                                           "failure:parse_opaque/parse_opaque:expected_error at parse_opaque: "
                                           "expected-like object reported an error",
                                       }));

  auto diagnostic_error_engine = pb::compile<DiagnosticErrorPipeline>(pb::runtime::sequential{});
  recording_observer diagnostic_observer{};
  diagnostic_error_engine.set_observer(&diagnostic_observer);

  auto diagnostic_failed = diagnostic_error_engine.try_run(Input{-5});
  assert(!diagnostic_failed.has_value());
  assert(diagnostic_failed.error().category == pb::runtime::error_category::expected_error);
  assert(diagnostic_failed.error().stage.key == "parse_diagnostic");
  assert(diagnostic_failed.error().stage.name == "parse_diagnostic");
  assert(diagnostic_failed.error().message == "diagnostic parse failed");
  assert(pb::runtime::describe(diagnostic_failed.error()) ==
         "expected_error at parse_diagnostic: diagnostic parse failed");
  assert((diagnostic_observer.events == std::vector<std::string>{
                                               "start:parse_diagnostic/parse_diagnostic",
                                               "failure:parse_diagnostic/parse_diagnostic:expected_error at "
                                               "parse_diagnostic: diagnostic parse failed",
                                           }));

  auto void_expected_engine = pb::compile<VoidExpectedPipeline>(pb::runtime::sequential{});
  recording_observer void_observer{};
  void_expected_engine.set_observer(&void_observer);

  auto void_ok = void_expected_engine.try_run(Input{5});
  assert(void_ok.has_value());
  assert(!void_ok.has_error());

  auto void_failed = void_expected_engine.try_run(Input{-5});
  assert(!void_failed.has_value());
  assert(void_failed.error().category == pb::runtime::error_category::expected_error);
  assert(void_failed.error().stage.key == "consume_void");
  assert(void_failed.error().stage.name == "consume_void");
  assert(void_failed.error().message == "expected-like object reported an error");
  assert(pb::runtime::describe(void_failed.error()) ==
         "expected_error at consume_void: expected-like object reported an error");
  assert((void_observer.events == std::vector<std::string>{
                                        "start:consume_void/consume_void",
                                        "success:consume_void/consume_void",
                                        "start:consume_void/consume_void",
                                        "failure:consume_void/consume_void:expected_error at consume_void: "
                                        "expected-like object reported an error",
                                    }));

  auto void_raw_ok = void_expected_engine.run(Input{5});
  assert(void_raw_ok.has_value());
  assert(!void_raw_ok.has_error());

  auto void_raw_failed = void_expected_engine.run(Input{-5});
  assert(!void_raw_failed.has_value());
  assert(void_raw_failed.error().category == pb::runtime::error_category::expected_error);
  assert(void_raw_failed.error().stage.key == "consume_void");
  assert(void_raw_failed.error().stage.name == "consume_void");
  assert(void_raw_failed.error().message == "expected-like object reported an error");

  auto void_move_only_engine = pb::compile<VoidMoveOnlyExpectedPipeline>(pb::runtime::sequential{});
  recording_observer void_move_only_observer{};
  void_move_only_engine.set_observer(&void_move_only_observer);

  auto void_move_only_failed = void_move_only_engine.try_run(Input{-5});
  assert(!void_move_only_failed.has_value());
  assert(void_move_only_failed.error().category == pb::runtime::error_category::expected_error);
  assert(void_move_only_failed.error().stage.key == "consume_void_move_only");
  assert(void_move_only_failed.error().stage.name == "consume_void_move_only");
  assert(void_move_only_failed.error().message == "expected-like object reported an error");
  assert(pb::runtime::describe(void_move_only_failed.error()) ==
         "expected_error at consume_void_move_only: expected-like object reported an error");
  assert((void_move_only_observer.events == std::vector<std::string>{
                                                  "start:consume_void_move_only/consume_void_move_only",
                                                  "failure:consume_void_move_only/consume_void_move_only:expected_error "
                                                  "at consume_void_move_only: expected-like object reported an error",
                                              }));

  auto void_move_only_raw_failed = void_move_only_engine.run(Input{-5});
  assert(!void_move_only_raw_failed.has_value());
  assert(void_move_only_raw_failed.error().category == pb::runtime::error_category::expected_error);
  assert(void_move_only_raw_failed.error().stage.key == "consume_void_move_only");
  assert(void_move_only_raw_failed.error().stage.name == "consume_void_move_only");
  assert(void_move_only_raw_failed.error().message == "expected-like object reported an error");

  auto direct_member_void_engine = pb::compile<DirectMemberVoidExpectedPipeline>(pb::runtime::sequential{});
  recording_observer direct_member_void_observer{};
  direct_member_void_engine.set_observer(&direct_member_void_observer);

  auto direct_member_void_ok = direct_member_void_engine.try_run(Input{5});
  assert(direct_member_void_ok.has_value());
  assert(!direct_member_void_ok.has_error());

  auto direct_member_void_failed = direct_member_void_engine.try_run(Input{-5});
  assert(!direct_member_void_failed.has_value());
  assert(direct_member_void_failed.error().category == pb::runtime::error_category::expected_error);
  assert(direct_member_void_failed.error().stage.key == "direct_member_consume_void");
  assert(direct_member_void_failed.error().stage.name == "direct_member_consume_void");
  assert(direct_member_void_failed.error().message == "member consume failed");
  assert(pb::runtime::describe(direct_member_void_failed.error()) ==
         "expected_error at direct_member_consume_void: member consume failed");
  assert((direct_member_void_observer.events == std::vector<std::string>{
                                                   "start:direct_member_consume_void/direct_member_consume_void",
                                                   "success:direct_member_consume_void/direct_member_consume_void",
                                                   "start:direct_member_consume_void/direct_member_consume_void",
                                                   "failure:direct_member_consume_void/direct_member_consume_void:"
                                                   "expected_error at direct_member_consume_void: member consume failed",
                                               }));

  auto direct_member_void_raw_ok = direct_member_void_engine.run(Input{5});
  assert(direct_member_void_raw_ok.has_value());
  assert(!direct_member_void_raw_ok.has_error());

  auto direct_member_void_raw_failed = direct_member_void_engine.run(Input{-5});
  assert(!direct_member_void_raw_failed.has_value());
  assert(direct_member_void_raw_failed.error().category == pb::runtime::error_category::expected_error);
  assert(direct_member_void_raw_failed.error().stage.key == "direct_member_consume_void");
  assert(direct_member_void_raw_failed.error().stage.name == "direct_member_consume_void");
  assert(direct_member_void_raw_failed.error().message == "member consume failed");

  auto failed = engine.run(Input{-2});
  assert(!failed.has_value());
  assert(failed.error().stage.key == "emit");
  assert(failed.error().stage.name == "emit_result");
  assert(failed.error().category == pb::runtime::error_category::stage_failure);
  assert(failed.error().message == "invalid parsed value");

  auto throwing_result_engine = pb::compile<ThrowingPipeline>(pb::runtime::sequential{});
  recording_observer observer{};
  throwing_result_engine.set_observer(&observer);

  auto result_caught = throwing_result_engine.try_run(Input{-200});
  assert(!result_caught.has_value());
  assert(result_caught.error().category == pb::runtime::error_category::exception);
  assert(result_caught.error().stage.name == "parse_input");
  assert(result_caught.error().stage.key == "parse_input");
  assert(result_caught.error().message == "parse_input_throwing failed");

  auto throwing_raw_engine = pb::compile<ThrowingPipelineRaw>(pb::runtime::sequential{});
  throwing_raw_engine.set_observer(&observer);

  try {
    (void)throwing_raw_engine.run(Input{-200});
    return 1;
  } catch (const std::runtime_error& error) {
    assert(std::string_view{error.what()} == "parse_input_throwing failed");
  }

  assert((observer.events == std::vector<std::string>{
              "start:parse_input/parse_input",
              "exception:parse_input/parse_input:parse_input_throwing failed",
              "start:parse_input/parse_input",
              "exception:parse_input/parse_input:parse_input_throwing failed",
          }));

  return 0;
}
