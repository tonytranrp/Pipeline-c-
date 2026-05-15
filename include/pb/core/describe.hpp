#pragma once

#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/stage_traits.hpp"
#include "pb/core/validate.hpp"

namespace pb::core {

template <std::size_t Index, class StageType>
struct stage_descriptor {
  using stage_type = StageType;
  using info = stage_info<StageType>;
  using input_type = typename info::input_type;
  using output_type = typename info::output_type;
  using error_type = typename info::error_type;

  static constexpr std::size_t index = Index;

  [[nodiscard]] static constexpr std::string_view name() noexcept {
    return info::name();
  }
};

template <class Pipeline>
struct pipeline_traits;

template <class Input, class Output>
struct pipeline_traits<pipeline<Input, Output, meta::type_list<>>> {
  using input_type = Input;
  using output_type = Output;
  using stages = meta::type_list<>;

  static constexpr std::size_t stage_count = 0;
  static constexpr bool empty = true;

  template <std::size_t Index>
  using stage_type = meta::at_t<stages, Index>;

  template <std::size_t Index>
  using stage = stage_descriptor<Index, stage_type<Index>>;
};

template <class Input, class Output, class FirstStage, class... RestStages>
struct pipeline_traits<pipeline<Input, Output, meta::type_list<FirstStage, RestStages...>>> {
  using input_type = Input;
  using output_type = Output;
  using stages = meta::type_list<FirstStage, RestStages...>;
  using first_stage_type = FirstStage;
  using last_stage_type = meta::back_t<stages>;

  static constexpr std::size_t stage_count = 1 + sizeof...(RestStages);
  static constexpr bool empty = false;

  template <std::size_t Index>
  using stage_type = meta::at_t<stages, Index>;

  template <std::size_t Index>
  using stage = stage_descriptor<Index, stage_type<Index>>;

  using first_stage = stage<0>;
  using last_stage = stage<stage_count - 1>;
};

namespace detail {
template <class... Stages, std::size_t... Indexes>
[[nodiscard]] constexpr auto stage_names(meta::type_list<Stages...>, std::index_sequence<Indexes...>) noexcept
    -> std::array<std::string_view, sizeof...(Stages)> {
  return {stage_descriptor<Indexes, Stages>::name()...};
}
} // namespace detail

template <ValidPipeline Pipeline>
inline constexpr std::size_t pipeline_size_v = pipeline_traits<Pipeline>::stage_count;

template <ValidPipeline Pipeline>
using pipeline_input_t = typename pipeline_traits<Pipeline>::input_type;

template <ValidPipeline Pipeline>
using pipeline_output_t = typename pipeline_traits<Pipeline>::output_type;

template <ValidPipeline Pipeline, std::size_t Index>
using pipeline_stage_t = typename pipeline_traits<Pipeline>::template stage_type<Index>;

template <ValidPipeline Pipeline, std::size_t Index>
using pipeline_stage_descriptor_t = typename pipeline_traits<Pipeline>::template stage<Index>;

template <ValidPipeline Pipeline>
struct pipeline_descriptor {
  using pipeline_type = Pipeline;
  using traits = pipeline_traits<Pipeline>;
  using input_type = typename traits::input_type;
  using output_type = typename traits::output_type;
  using stages = typename traits::stages;

  static constexpr std::size_t stage_count = traits::stage_count;
  static constexpr bool empty = traits::empty;

  template <std::size_t Index>
  using stage = typename traits::template stage<Index>;

  template <std::size_t Index>
  using stage_type = typename traits::template stage_type<Index>;

  template <std::size_t Index>
  [[nodiscard]] static constexpr std::string_view stage_name() noexcept {
    return stage<Index>::name();
  }

  [[nodiscard]] static constexpr auto stage_names() noexcept -> std::array<std::string_view, stage_count> {
    return detail::stage_names(stages{}, std::make_index_sequence<stage_count>{});
  }
};

template <ValidPipeline Pipeline>
[[nodiscard]] constexpr auto describe() noexcept -> pipeline_descriptor<Pipeline> {
  return {};
}

} // namespace pb::core
