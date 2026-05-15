#pragma once

#include <concepts>

#include "pb/core/stage_traits.hpp"

namespace pb::core {

template <typename T>
concept stage = stage_traits<T>::is_valid_stage;

template <typename T>
concept pipeline_source = stage<T> && std::same_as<typename stage_traits<T>::input_type, void>;

template <typename T>
concept pipeline_sink = stage<T> && std::same_as<typename stage_traits<T>::output_type, void>;

template <typename From, typename To>
concept connectable =
    stage<From> && stage<To> &&
    std::same_as<typename stage_traits<From>::output_type, typename stage_traits<To>::input_type>;

template <typename From, typename To>
concept source_to_stage =
    stage<From> && stage<To> &&
    std::same_as<typename stage_traits<From>::output_type, typename stage_traits<To>::input_type>;

template <typename From, typename To>
concept stage_to_sink =
    stage<From> && stage<To> && pipeline_sink<To> &&
    std::same_as<typename stage_traits<From>::output_type, typename stage_traits<To>::input_type>;

}  // namespace pb::core
