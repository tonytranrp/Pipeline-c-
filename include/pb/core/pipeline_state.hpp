#pragma once

#include <concepts>
#include <type_traits>

#include "pb/core/concepts.hpp"
#include "pb/core/meta.hpp"

namespace pb::core {

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

template <class State, class Output>
struct finalize_pipeline;

template <class Input, class Current, class... Stages, class Output>
struct finalize_pipeline<pipeline_state<Input, Current, Stages...>, Output> {
  static_assert(std::same_as<Current, Output>,
                "Pipeline sink mismatch: actual final output type does not match requested sink type");
  using type = pipeline<Input, Output, meta::type_list<Stages...>>;
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
};

template <class Input>
using from = pipeline_state<Input, Input>;

} // namespace pb::core
