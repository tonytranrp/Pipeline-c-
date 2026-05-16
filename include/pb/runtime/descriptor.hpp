#pragma once

#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

#include "pb/core/describe.hpp"
#include "pb/core/validate.hpp"

namespace pb::runtime {

inline constexpr std::string_view descriptor_schema_version = "pb.runtime.descriptor.v1";

enum class descriptor_topology {
  linear,
};

struct descriptor_stage_record {
  std::size_t index{};
  std::string_view key{};
  std::string_view name{};
};

struct descriptor_edge_record {
  std::size_t index{};
  std::size_t from_stage_index{};
  std::size_t to_stage_index{};
  std::string_view from_key{};
  std::string_view from_name{};
  std::string_view to_key{};
  std::string_view to_name{};
};

template <std::size_t StageCount>
struct descriptor_view {
  static constexpr std::string_view schema_version = descriptor_schema_version;
  static constexpr descriptor_topology topology = descriptor_topology::linear;
  static constexpr std::size_t stage_count = StageCount;
  static constexpr std::size_t edge_count = StageCount > 0 ? StageCount - 1 : 0;
  static constexpr bool empty = StageCount == 0;

  std::array<descriptor_stage_record, StageCount> stages{};
  std::array<descriptor_edge_record, edge_count> edges{};

  [[nodiscard]] constexpr auto stage_records() const noexcept
      -> const std::array<descriptor_stage_record, StageCount>& {
    return stages;
  }

  [[nodiscard]] constexpr auto edge_records() const noexcept
      -> const std::array<descriptor_edge_record, edge_count>& {
    return edges;
  }
};

namespace detail {
[[nodiscard]] constexpr auto to_runtime_stage_record(pb::core::stage_record source) noexcept
    -> descriptor_stage_record {
  return descriptor_stage_record{.index = source.index, .key = source.key, .name = source.name};
}

[[nodiscard]] constexpr auto to_runtime_edge_record(pb::core::edge_record source) noexcept -> descriptor_edge_record {
  return descriptor_edge_record{.index = source.index,
                                .from_stage_index = source.from_stage_index,
                                .to_stage_index = source.to_stage_index,
                                .from_key = source.from_key,
                                .from_name = source.from_name,
                                .to_key = source.to_key,
                                .to_name = source.to_name};
}

template <std::size_t StageCount, std::size_t... StageIndexes, std::size_t... EdgeIndexes>
[[nodiscard]] constexpr auto make_runtime_descriptor(pb::core::pipeline_descriptor_view<StageCount> source,
                                                     std::index_sequence<StageIndexes...>,
                                                     std::index_sequence<EdgeIndexes...>) noexcept
    -> descriptor_view<StageCount> {
  return descriptor_view<StageCount>{
      .stages = {to_runtime_stage_record(source.stage_records()[StageIndexes])...},
      .edges = {to_runtime_edge_record(source.edge_records()[EdgeIndexes])...},
  };
}
} // namespace detail

template <pb::core::ValidPipeline Pipeline>
[[nodiscard]] constexpr auto make_descriptor() noexcept -> descriptor_view<pb::core::pipeline_size_v<Pipeline>> {
  constexpr auto stage_count = pb::core::pipeline_size_v<Pipeline>;
  constexpr auto edge_count = pb::core::pipeline_edge_count_v<Pipeline>;
  return detail::make_runtime_descriptor(pb::core::descriptor_view<Pipeline>(),
                                         std::make_index_sequence<stage_count>{},
                                         std::make_index_sequence<edge_count>{});
}

} // namespace pb::runtime
