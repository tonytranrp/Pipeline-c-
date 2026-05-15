#pragma once

#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

#include "pb/core/meta.hpp"
#include "pb/core/stage_traits.hpp"
#include "pb/core/validate.hpp"

namespace pb::core {

struct stage_record {
  std::size_t index{};
  std::string_view name{};
};

template <std::size_t Index, class StageType>
struct stage_descriptor {
  using stage_type = StageType;
  using input_type = stage_input_t<StageType>;
  using output_type = stage_output_t<StageType>;
  using error_type = stage_error_t<StageType>;

  static constexpr std::size_t index = Index;

  [[nodiscard]] static constexpr std::string_view name() noexcept {
    return stage_traits<StageType>::name();
  }
};

template <class Pipeline>
struct pipeline_traits;

template <class Input, class Output, class... Stages>
struct pipeline_traits<pipeline<Input, Output, meta::type_list<Stages...>>> {
  using input_type = Input;
  using output_type = Output;
  using stages = meta::type_list<Stages...>;

  static constexpr std::size_t stage_count = sizeof...(Stages);
  static constexpr bool empty = stage_count == 0;

  template <std::size_t Index>
  using stage_type = meta::at_t<stages, Index>;

  template <std::size_t Index>
  using stage = stage_descriptor<Index, stage_type<Index>>;
};

namespace detail {
template <class... Stages, std::size_t... Indexes>
[[nodiscard]] constexpr auto stage_names(meta::type_list<Stages...>, std::index_sequence<Indexes...>) noexcept
    -> std::array<std::string_view, sizeof...(Stages)> {
  return {stage_descriptor<Indexes, Stages>::name()...};
}

template <class... Stages, std::size_t... Indexes>
[[nodiscard]] constexpr auto stage_records(meta::type_list<Stages...>, std::index_sequence<Indexes...>) noexcept
    -> std::array<stage_record, sizeof...(Stages)> {
  return {stage_record{Indexes, stage_descriptor<Indexes, Stages>::name()}...};
}
} // namespace detail

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

  [[nodiscard]] static constexpr auto stage_records() noexcept -> std::array<stage_record, stage_count> {
    return detail::stage_records(stages{}, std::make_index_sequence<stage_count>{});
  }
};

template <ValidPipeline Pipeline>
[[nodiscard]] constexpr auto describe() noexcept -> pipeline_descriptor<Pipeline> {
  return {};
}

} // namespace pb::core
