#pragma once

#include <concepts>
#include <type_traits>

#include "pb/core/stage_traits.hpp"

namespace pb::core {

template <class T>
concept Stage = stage_traits<T>::is_valid_stage;

template <class T>
concept stage = Stage<T>;

template <class From, class To>
concept Connectable = Stage<To> && std::same_as<From, stage_input_t<To>>;

template <class A, class B>
concept AdjacentStages = Stage<A> && Stage<B> && std::same_as<stage_output_t<A>, stage_input_t<B>>;

template <class StageType, class Input>
concept RunnableStage = Stage<StageType> && requires(StageType stage_value, Input input) {
  stage_value(input);
};

} // namespace pb::core
