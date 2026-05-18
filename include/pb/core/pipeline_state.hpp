#pragma once

#include <concepts>
#include <tuple>
#include <type_traits>
#include <variant>

#include "pb/core/concepts.hpp"
#include "pb/core/meta.hpp"

namespace pb::core {

template <class Predicate, class BranchStage>
struct branch_case;

template <class JoinStage>
struct join_node;

template <class... Cases>
struct branch_outputs;

template <class Case>
struct branch_case_output;

template <class Outputs, class Output>
struct branch_output_validation;

template <class Outputs, class JoinStage>
struct join_builder_validation;

template <class... Cases>
struct branch_raw_output_types;

template <class... Cases>
struct branch_unified_output;

namespace detail {

template <class>
inline constexpr bool always_false_v = false;

template <class Predicate, bool IsPredicateStage = Stage<Predicate>>
struct branch_predicate_output_bool : std::false_type {};

template <class Predicate>
struct branch_predicate_output_bool<Predicate, true>
    : std::bool_constant<std::convertible_to<stage_output_t<Predicate>, bool>> {};

template <class>
struct is_branch_case : std::false_type {};

template <class Predicate, class BranchStage>
struct is_branch_case<branch_case<Predicate, BranchStage>> : std::true_type {};

template <class>
struct is_join_node : std::false_type {};

template <class JoinStage>
struct is_join_node<join_node<JoinStage>> : std::true_type {};

template <class>
struct is_branch_outputs : std::false_type {};

template <class... Cases>
struct is_branch_outputs<branch_outputs<Cases...>> : std::true_type {};

template <class Output, class... Cases>
struct branch_outputs_match_output
    : std::bool_constant<(std::same_as<typename branch_case_output<Cases>::output_type, Output> && ...)> {};

template <class... Cases>
struct first_branch_output {
  using type = void;
};

template <class First, class... Rest>
struct first_branch_output<First, Rest...> {
  using type = typename branch_case_output<First>::output_type;
};

template <class Case, bool IsBranchCase = is_branch_case<Case>::value>
struct branch_case_output_impl {
  static_assert(always_false_v<Case>,
                "Branch output marker requires pb::case_<Predicate>::then<Stage>");
};

template <class Case>
struct branch_case_output_impl<Case, true> {
  using case_type = Case;
  using output_type = stage_output_t<typename Case::stage_type>;
};

template <class Join, bool IsJoinNode = is_join_node<Join>::value>
struct join_output_impl {
  static_assert(always_false_v<Join>, "Join output marker requires pb::join_node<Stage>");
};

template <class Join>
struct join_output_impl<Join, true> {
  using join_type = Join;
  using stage_type = typename Join::stage_type;
  using input_type = typename Join::input_type;
  using output_type = typename Join::output_type;
};

template <class Outputs, class Join, bool IsOutputs = is_branch_outputs<Outputs>::value,
          bool IsJoin = is_join_node<Join>::value>
struct join_validation_impl {
  static_assert(always_false_v<Outputs>,
                "Join validation requires pb::branch_outputs<...> and pb::join_node<Stage>");
};

template <class Outputs, class Join>
struct join_validation_impl<Outputs, Join, true, true> {
  static_assert(std::same_as<typename Join::input_type, typename Outputs::output_type>,
                "Join validation mismatch: join stage input_type must match the unified branch output_type");

  using branch_outputs_type = Outputs;
  using join_type = Join;
  using raw_output_types = typename Outputs::output_types;
  using input_type = typename Outputs::output_type;
  using output_type = typename Join::output_type;
};

template <class Outputs, class JoinStage, bool IsOutputs = is_branch_outputs<Outputs>::value,
          bool IsJoinStage = Stage<JoinStage>>
struct join_builder_validation_impl {
  static_assert(always_false_v<Outputs>,
                "Join builder validation requires pb::branch_outputs<...> and a valid join stage");
};

template <class Outputs, class JoinStage>
struct join_builder_validation_impl<Outputs, JoinStage, true, true> {
  using join_type = join_node<JoinStage>;

  static_assert(std::same_as<stage_input_t<JoinStage>, typename Outputs::output_type>,
                "Join builder source mismatch: join stage input_type must match unified branch output_type before "
                "pb::from<...>::branch<...>::join<Stage>");

  using branch_outputs_type = Outputs;
  using stage_type = JoinStage;
  using raw_output_types = typename Outputs::output_types;
  using input_type = typename Outputs::output_type;
  using output_type = stage_output_t<JoinStage>;
};

template <class Outputs, class Output, bool IsOutputs = is_branch_outputs<Outputs>::value>
struct branch_output_validation_impl {
  static_assert(always_false_v<Outputs>,
                "Branch output validation requires pb::branch_outputs<...>");
};

template <class Output, class... Cases>
struct branch_output_validation_impl<branch_outputs<Cases...>, Output, true> {
  static_assert(branch_outputs_match_output<Output, Cases...>::value,
                "Branch output validation mismatch: every branch output_type must match requested output_type");

  using branch_outputs_type = branch_outputs<Cases...>;
  using input_type = typename branch_outputs<Cases...>::input_type;
  using output_type = Output;
  using output_types = typename branch_outputs<Cases...>::output_types;
  static constexpr std::size_t output_count = sizeof...(Cases);
};

template <class... Cases>
struct branch_cases_valid : std::bool_constant<(is_branch_case<Cases>::value && ...)> {};

template <bool AllCasesValid, class... Cases>
struct branch_cases_same_input_impl : std::false_type {};

template <class... Cases>
struct branch_cases_same_input_impl<true, Cases...> : std::true_type {};

template <class First, class... Rest>
struct branch_cases_same_input_impl<true, First, Rest...>
    : std::bool_constant<(std::same_as<typename First::input_type, typename Rest::input_type> && ...)> {};

template <class... Cases>
struct branch_cases_same_input
    : branch_cases_same_input_impl<branch_cases_valid<Cases...>::value, Cases...> {};

template <bool IsBranchCase, class Case>
struct branch_case_input_or_void {
  using type = void;
};

template <class Case>
struct branch_case_input_or_void<true, Case> {
  using type = typename Case::input_type;
};

template <class... Cases>
struct branch_node_input {
  using type = void;
};

template <class First, class... Rest>
struct branch_node_input<First, Rest...>
    : branch_case_input_or_void<is_branch_case<First>::value, First> {};

template <class T, class... Rest>
struct all_same : std::bool_constant<(std::same_as<T, Rest> && ...)> {};

template <class... Outputs>
struct branch_output_type_or_variant;

template <class Output>
struct branch_output_type_or_variant<Output> {
  using type = Output;
};

template <class Output1, class Output2, class... Rest>
struct branch_output_type_or_variant<Output1, Output2, Rest...> {
private:
  static constexpr bool homogeneous = all_same<Output1, Output2, Rest...>::value;

  template <bool IsHomogeneous, class... Ts>
  struct impl;

  template <class T, class... Ts>
  struct impl<true, T, Ts...> {
    using type = T;
  };

  template <class... Ts>
  struct impl<false, Ts...> {
    using type = std::variant<Ts...>;
  };

public:
  using type = typename impl<homogeneous, Output1, Output2, Rest...>::type;
};

template <class OutputTypes>
struct branch_unified_output_from_type_list;

template <class... Outputs>
struct branch_unified_output_from_type_list<meta::type_list<Outputs...>> {
  using type = typename branch_output_type_or_variant<Outputs...>::type;
};

template <class... Cases>
struct selected_branch_node;

template <class>
struct is_selected_branch_node : std::false_type {};

template <class... Cases>
struct is_selected_branch_node<selected_branch_node<Cases...>> : std::true_type {};

template <class... Stages>
struct last_stage_or_void {
  using type = void;
};

template <class First, class... Rest>
struct last_stage_or_void<First, Rest...> {
  using type = meta::back_t<meta::type_list<First, Rest...>>;
};
} // namespace detail

template <class Predicate, class BranchStage>
struct branch_case {
  static_assert(Stage<Predicate>,
                "Branch predicate is invalid: define input_type and bool-like output_type");
  static_assert(detail::branch_predicate_output_bool<Predicate>::value,
                "Branch predicate stage must produce a bool-like output_type");
  static_assert(Stage<BranchStage>, "Branch case target stage is invalid: define input_type and output_type");
  static_assert(std::same_as<stage_input_t<Predicate>, stage_input_t<BranchStage>>,
                "Branch case source mismatch: predicate input_type must match branch target stage input_type");

  using predicate_type = Predicate;
  using stage_type = BranchStage;
  using input_type = stage_input_t<Predicate>;
};

template <class Case>
struct branch_case_output : detail::branch_case_output_impl<Case> {};

template <class Predicate>
struct case_ {
  template <class Stage>
  using then = branch_case<Predicate, Stage>;
};

template <class... Cases>
struct branch_node {
  static_assert(sizeof...(Cases) > 0, "Branch node requires at least one pb::case_<Predicate>::then<Stage>");
  static_assert(detail::branch_cases_valid<Cases...>::value,
                "Branch node requires pb::case_<Predicate>::then<Stage> cases");
  static_assert(detail::branch_cases_same_input<Cases...>::value,
                "Branch node source mismatch: all branch cases must share input_type");

  using cases = meta::type_list<Cases...>;
  using input_type = typename detail::branch_node_input<Cases...>::type;
  using output_types = meta::type_list<typename branch_case_output<Cases>::output_type...>;
  static constexpr std::size_t case_count = sizeof...(Cases);
};

template <class... Cases>
struct branch_outputs {
  static_assert(sizeof...(Cases) > 0, "Branch outputs require at least one pb::case_<Predicate>::then<Stage>");
  static_assert(detail::branch_cases_valid<Cases...>::value,
                "Branch outputs require pb::case_<Predicate>::then<Stage> cases");
  static_assert(detail::branch_cases_same_input<Cases...>::value,
                "Branch outputs source mismatch: all branch cases must share input_type");

  using cases = meta::type_list<Cases...>;
  using input_type = typename detail::branch_node_input<Cases...>::type;
  using output_types = meta::type_list<typename branch_case_output<Cases>::output_type...>;
  using output_type = typename detail::branch_unified_output_from_type_list<output_types>::type;
  static constexpr std::size_t output_count = sizeof...(Cases);
};

template <class... Cases>
struct branch_raw_output_types {
  using type = typename branch_outputs<Cases...>::output_types;
};

template <class... Cases>
using branch_raw_output_types_t = typename branch_raw_output_types<Cases...>::type;

template <class... Cases>
struct branch_unified_output {
  using type = typename branch_outputs<Cases...>::output_type;
};

template <class... Cases>
using branch_unified_output_t = typename branch_unified_output<Cases...>::type;

namespace detail {
template <class... Cases>
struct selected_branch_node {
  // Extract all branch output types
  using cases = meta::type_list<Cases...>;
  using input_type = typename branch_node_input<Cases...>::type;

  // Collect all output types via branch_outputs (validates cases are valid branch_case types)
  using branch_outputs_type = branch_outputs<Cases...>;
  using output_types = typename branch_outputs_type::output_types;

  // Variant-or-single type:
  // - If all branch outputs are the same type, use it directly (homogeneous, backward compatible).
  // - Otherwise, wrap in std::variant (heterogeneous).
  using output_type = typename branch_outputs_type::output_type;

  static constexpr std::size_t case_count = sizeof...(Cases);
  static constexpr bool homogeneous = all_same<
      typename branch_case_output<Cases>::output_type...>::value;

  // For move-only inputs, predicates must accept const& to inspect the input
  // before it gets moved into the selected branch stage.
  // For copy-constructible inputs, no additional predicate constraint is required.
  static_assert(std::copy_constructible<input_type> ||
                    (std::is_invocable_v<typename Cases::predicate_type, const input_type&> && ...),
                "Move-only branch inputs require predicates callable with const input_type& — "
                "predicates inspect the input by const reference while the selected branch stage "
                "receives it by move; define operator()(const input_type&) const on each predicate stage");

  // Storage for stateful branch execution: preserves predicate and branch stage
  // instances across multiple pipeline runs so they can carry mutable state.
  using predicate_tuple = std::tuple<typename Cases::predicate_type...>;
  using branch_stage_tuple = std::tuple<typename Cases::stage_type...>;

  predicate_tuple predicates_{};
  branch_stage_tuple branch_stages_{};

  [[nodiscard]] static constexpr auto stage_name() noexcept { return "branch"; }
  [[nodiscard]] static constexpr auto stage_key() noexcept { return "branch"; }
};
} // namespace detail

template <class Outputs, class Output>
struct branch_output_validation : detail::branch_output_validation_impl<Outputs, Output> {};

template <class JoinStage>
struct join_node {
  static_assert(Stage<JoinStage>, "Join stage is invalid: define input_type and output_type");

  using stage_type = JoinStage;
  using input_type = stage_input_t<JoinStage>;
  using output_type = stage_output_t<JoinStage>;
};

template <class Join>
struct join_output : detail::join_output_impl<Join> {};

template <class Outputs, class Join>
struct join_validation : detail::join_validation_impl<Outputs, Join> {};

template <class Outputs, class JoinStage>
struct join_builder_validation : detail::join_builder_validation_impl<Outputs, JoinStage> {};

template <class Input, class Output, class StageList>
struct pipeline {
  using input_type = Input;
  using output_type = Output;
  using stages = StageList;
  static constexpr bool valid = true;
};

template <class Input, class Current, class... Stages>
struct pipeline_state;

namespace detail {

template <class State, class Stage>
struct append_stage;

template <class Input, class Current, class... Stages, class StageType>
struct append_stage<pipeline_state<Input, Current, Stages...>, StageType> {
  static_assert(Stage<StageType>, "Pipeline stage is invalid: define input_type and output_type");
  static_assert(Connectable<Current, StageType>,
                "Pipeline edge mismatch: previous output_type (or pipeline input) must exactly match next "
                "stage input_type; inspect pb::connectable_v<PreviousOutput, NextStage> or "
                "pb::AdjacentStages<PreviousStage, NextStage>");
  using type = pipeline_state<Input, stage_output_t<StageType>, Stages..., StageType>;
};

template <class State, class JoinStage>
struct append_join;

template <class Input, class Current, class... Stages, class JoinStage>
struct append_join<pipeline_state<Input, Current, Stages...>, JoinStage> {
  using last_stage = typename detail::last_stage_or_void<Stages...>::type;

  static_assert(!std::is_same_v<last_stage, void>,
                "Join requires a preceding branch: use pb::from<...>::branch<...>::join<Stage>");

  static_assert(detail::is_selected_branch_node<last_stage>::value,
                "Join must follow a branch node: pipeline_state::join is only valid after ::branch<...>");

  static_assert(Stage<JoinStage>, "Join stage is invalid: define input_type and output_type");

  static_assert(Connectable<Current, JoinStage>,
                "Pipeline edge mismatch: previous output_type must match join stage input_type; inspect "
                "pb::connectable_v<PreviousOutput, JoinStage>");

  using type = pipeline_state<Input, stage_output_t<JoinStage>, Stages..., JoinStage>;
};

template <class State, class Output>
struct finalize_pipeline;

template <class Input, class Current, class... Stages, class Output>
struct finalize_pipeline<pipeline_state<Input, Current, Stages...>, Output> {
  static_assert(std::same_as<Current, Output>,
                "Pipeline sink mismatch: actual final output type does not match requested sink type");
  using type = pipeline<Input, Output, meta::type_list<Stages...>>;
};

template <class State, class... Cases>
struct append_branch;

template <class Input, class Current, class... Stages>
struct append_branch<pipeline_state<Input, Current, Stages...>> {
  static_assert(always_false_v<pipeline_state<Input, Current, Stages...>>,
                "Branch node requires at least one pb::case_<Predicate>::then<Stage>");
};

template <class Input, class Current, class... Stages, class... Cases>
struct append_branch<pipeline_state<Input, Current, Stages...>, Cases...> {
  using branch_input = typename branch_node_input<Cases...>::type;

  static_assert(std::same_as<Current, branch_input>,
                "Branch builder source mismatch: current pipeline output_type must match branch case input_type "
                "before pb::from<...>::branch<...>");

  using branch_stage = selected_branch_node<Cases...>;
  using type = pipeline_state<Input, typename branch_stage::output_type, Stages..., branch_stage>;
};

} // namespace detail

template <class Input, class Current, class... Stages>
struct pipeline_state {
  using input_type = Input;
  using current_type = Current;
  using stages = meta::type_list<Stages...>;

  template <class StageType>
  using then = typename detail::append_stage<pipeline_state, StageType>::type;

  template <class StageType>
  using join = typename detail::append_join<pipeline_state, StageType>::type;

  template <class Output>
  using to = typename detail::finalize_pipeline<pipeline_state, Output>::type;

  template <class... Cases>
  using branch = typename detail::append_branch<pipeline_state, Cases...>::type;

  /// Policy tag — pass-through that carries policy type metadata.
  /// Policies are stored as type-level annotations; the compiler eliminates
  /// every tag at zero runtime cost.  Future extensions will inspect the
  /// policy pack to drive compile-time behavioural selection.
  template <class... Policies>
  using with = pipeline_state;
};

template <class Input>
using from = pipeline_state<Input, Input>;

} // namespace pb::core
