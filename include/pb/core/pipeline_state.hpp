#pragma once

#include <concepts>
#include <type_traits>

#include "pb/core/concepts.hpp"
#include "pb/core/meta.hpp"

namespace pb::core {

namespace detail {
template <class Predicate, bool IsPredicateStage = Stage<Predicate>>
struct branch_predicate_output_bool : std::false_type {};

template <class Predicate>
struct branch_predicate_output_bool<Predicate, true>
    : std::bool_constant<std::convertible_to<stage_output_t<Predicate>, bool>> {};
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

template <class Predicate>
struct case_ {
  template <class Stage>
  using then = branch_case<Predicate, Stage>;
};

template <class... Cases>
struct branch_node {
  using cases = meta::type_list<Cases...>;
  static constexpr std::size_t case_count = sizeof...(Cases);
};

template <class JoinStage>
struct join_node {
  static_assert(Stage<JoinStage>, "Join stage is invalid: define input_type and output_type");

  using stage_type = JoinStage;
  using input_type = stage_input_t<JoinStage>;
  using output_type = stage_output_t<JoinStage>;
};

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

template <class>
inline constexpr bool always_false_v = false;

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

template <class State, class Output>
struct finalize_pipeline;

template <class Input, class Current, class... Stages, class Output>
struct finalize_pipeline<pipeline_state<Input, Current, Stages...>, Output> {
  static_assert(std::same_as<Current, Output>,
                "Pipeline sink mismatch: actual final output type does not match requested sink type");
  using type = pipeline<Input, Output, meta::type_list<Stages...>>;
};

template <class State, class... Cases>
struct unsupported_branch;

template <class Input, class Current, class... Stages, class... Cases>
struct unsupported_branch<pipeline_state<Input, Current, Stages...>, Cases...> {
  using requested_node = branch_node<Cases...>;

  static_assert(always_false_v<requested_node>,
                "Branch/join topology is not implemented yet: Phase 5 currently exposes only "
                "pb::case_/pb::branch_node marker types, while pb::from<...>::branch<...> remains an "
                "unsupported boundary");
  using type = void;
};

} // namespace detail

template <class Input, class Current, class... Stages>
struct pipeline_state {
  using input_type = Input;
  using current_type = Current;
  using stages = meta::type_list<Stages...>;

  template <class StageType>
  using then = typename detail::append_stage<pipeline_state, StageType>::type;

  template <class Output>
  using to = typename detail::finalize_pipeline<pipeline_state, Output>::type;

  template <class... Cases>
  using branch = typename detail::unsupported_branch<pipeline_state, Cases...>::type;
};

template <class Input>
using from = pipeline_state<Input, Input>;

} // namespace pb::core
