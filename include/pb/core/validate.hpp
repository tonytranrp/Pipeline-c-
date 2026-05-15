#pragma once

#include "pb/core/pipeline_state.hpp"

namespace pb::core {

template <class T>
struct is_pipeline : std::false_type {};

template <class Input, class Output, class Stages>
struct is_pipeline<pipeline<Input, Output, Stages>> : std::true_type {};

template <class T>
inline constexpr bool is_pipeline_v = is_pipeline<T>::value;

template <class T>
concept ValidPipeline = is_pipeline_v<T> && T::valid;

template <class T>
inline constexpr bool valid = ValidPipeline<T>;

} // namespace pb::core
