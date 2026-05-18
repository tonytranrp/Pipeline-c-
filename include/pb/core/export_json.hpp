#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

#include "pb/core/describe.hpp"
#include "pb/core/pipeline_state.hpp"

namespace pb::core {

namespace detail {

template <class StageList>
struct json_has_branch_stage_impl;

template <class... Stages>
struct json_has_branch_stage_impl<meta::type_list<Stages...>>
    : std::bool_constant<(is_selected_branch_node<Stages>::value || ...)> {};

template <class Pipeline>
inline constexpr bool json_pipeline_has_branch_v =
    json_has_branch_stage_impl<typename pipeline_traits<Pipeline>::stages>::value;

inline void append_json_string(std::ostringstream& stream, std::string_view value) {
  stream << '"';
  for (const auto ch : value) {
    switch (ch) {
    case '"':
      stream << "\\\"";
      break;
    case '\\':
      stream << "\\\\";
      break;
    case '\n':
      stream << "\\n";
      break;
    case '\r':
      stream << "\\r";
      break;
    case '\t':
      stream << "\\t";
      break;
    default:
      stream << ch;
      break;
    }
  }
  stream << '"';
}

template <class Case>
void append_branch_case_json(std::ostringstream& stream, std::size_t index) {
  using Predicate = typename Case::predicate_type;
  using BranchStage = typename Case::stage_type;

  stream << "{\"index\":" << index << ",\"predicate_key\":";
  append_json_string(stream, stage_traits<Predicate>::key());
  stream << ",\"predicate_name\":";
  append_json_string(stream, stage_traits<Predicate>::name());
  stream << ",\"stage_key\":";
  append_json_string(stream, stage_traits<BranchStage>::key());
  stream << ",\"stage_name\":";
  append_json_string(stream, stage_traits<BranchStage>::name());
  stream << ",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"}";
  stream << ",\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}";
  stream << "}";
}

template <class... Cases, std::size_t... Indexes>
void append_branch_cases_json(std::ostringstream& stream, meta::type_list<Cases...>, std::index_sequence<Indexes...>) {
  stream << ",\"branch_cases\":[";
  ((stream << (Indexes == 0 ? "" : ","), append_branch_case_json<Cases>(stream, Indexes)), ...);
  stream << "]";
}

template <class Stage>
void append_stage_kind_json(std::ostringstream& stream) {
  if constexpr (is_selected_branch_node<Stage>::value) {
    stream << ",\"kind\":\"branch\"";
    append_branch_cases_json(stream, typename Stage::cases{}, std::make_index_sequence<Stage::case_count>{});
  } else {
    stream << ",\"kind\":\"stage\"";
  }
}

template <class Pipeline, std::size_t Index>
void append_graph_stage_json(std::ostringstream& stream) {
  using Stage = pipeline_stage_t<Pipeline, Index>;
  using Descriptor = pipeline_stage_descriptor_t<Pipeline, Index>;

  stream << "{\"index\":" << Index << ",\"key\":";
  append_json_string(stream, Descriptor::key());
  stream << ",\"name\":";
  append_json_string(stream, Descriptor::name());
  append_stage_kind_json<Stage>(stream);
  stream << "}";
}

template <class Pipeline, std::size_t... Indexes>
void append_graph_stages_json(std::ostringstream& stream, std::index_sequence<Indexes...>) {
  ((stream << (Indexes == 0 ? "" : ","), append_graph_stage_json<Pipeline, Indexes>(stream)), ...);
}

inline void append_graph_edge_json(std::ostringstream& stream, const edge_record& edge) {
  stream << "{\"index\":" << edge.index << ",\"from_stage_index\":" << edge.from_stage_index
         << ",\"to_stage_index\":" << edge.to_stage_index << ",\"from_key\":";
  append_json_string(stream, edge.from_key);
  stream << ",\"from_name\":";
  append_json_string(stream, edge.from_name);
  stream << ",\"to_key\":";
  append_json_string(stream, edge.to_key);
  stream << ",\"to_name\":";
  append_json_string(stream, edge.to_name);
  stream << "}";
}

} // namespace detail

template <ValidPipeline Pipeline>
[[nodiscard]] auto to_json() -> std::string {
  constexpr auto descriptor = describe<Pipeline>();
  constexpr auto topology = detail::json_pipeline_has_branch_v<Pipeline> ? std::string_view{"branch"}
                                                                         : std::string_view{"linear"};

  std::ostringstream stream;
  stream << "{\"schema_version\":\"pb.core.graph.v1\",\"topology\":";
  detail::append_json_string(stream, topology);
  stream << ",\"stage_count\":"
         << descriptor.stage_count << ",\"edge_count\":" << descriptor.edge_count << ",\"stages\":[";
  detail::append_graph_stages_json<Pipeline>(stream, std::make_index_sequence<descriptor.stage_count>{});
  stream << "],\"edges\":[";

  bool first_edge = true;
  for (const auto& edge : descriptor.edge_records()) {
    if (!first_edge) {
      stream << ",";
    }
    first_edge = false;
    detail::append_graph_edge_json(stream, edge);
  }

  stream << "]}";
  return stream.str();
}

} // namespace pb::core
