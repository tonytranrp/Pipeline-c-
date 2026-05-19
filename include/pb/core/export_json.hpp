#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

#include "pb/core/pipeline_state.hpp"
#include "pb/runtime/descriptor.hpp"

namespace pb::core {

namespace detail {

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

[[nodiscard]] inline auto branch_case_label_or_index(
    const pb::runtime::descriptor_branch_case_record& branch_case) -> std::string {
  if (!branch_case.case_label.empty()) {
    return std::string{branch_case.case_label};
  }
  return std::to_string(branch_case.case_index);
}

inline void append_branch_case_json(std::ostringstream& stream,
                                    const pb::runtime::descriptor_branch_case_record& branch_case) {
  stream << "{\"index\":" << branch_case.case_index << ",\"case_id\":";
  append_json_string(stream, branch_case.case_id);
  stream << ",\"case_key\":";
  append_json_string(stream, branch_case.case_key);
  stream << ",\"case_label\":";
  append_json_string(stream, branch_case_label_or_index(branch_case));
  stream << ",\"predicate_node_id\":";
  append_json_string(stream, branch_case.predicate_node_id);
  stream << ",\"stage_node_id\":";
  append_json_string(stream, branch_case.stage_node_id);
  stream << ",\"predicate_key\":";
  append_json_string(stream, branch_case.predicate_key);
  stream << ",\"predicate_name\":";
  append_json_string(stream, branch_case.predicate_name);
  stream << ",\"stage_key\":";
  append_json_string(stream, branch_case.stage_key);
  stream << ",\"stage_name\":";
  append_json_string(stream, branch_case.stage_name);
  stream << ",\"predicate_edge\":{\"from\":\"branch\",\"to\":\"predicate\",\"style\":\"dashed\",\"label\":\"test\"}";
  stream << ",\"stage_edge\":{\"from\":\"predicate\",\"to\":\"case_stage\"}";
  stream << "}";
}

template <class Descriptor>
void append_branch_cases_json(std::ostringstream& stream, const Descriptor& descriptor, std::size_t branch_stage_index) {
  stream << ",\"branch_cases\":[";
  bool first_case = true;
  for (const auto& branch_case : descriptor.branch_case_records()) {
    if (branch_case.branch_stage_index != branch_stage_index) {
      continue;
    }
    if (!first_case) {
      stream << ",";
    }
    first_case = false;
    append_branch_case_json(stream, branch_case);
  }
  stream << "]";
}

template <class Descriptor>
void append_stage_kind_json(std::ostringstream& stream, const Descriptor& descriptor,
                            const pb::runtime::descriptor_stage_record& stage) {
  if (stage.topology == pb::runtime::descriptor_topology::branch) {
    stream << ",\"kind\":\"branch\"";
    append_branch_cases_json(stream, descriptor, stage.index);
  } else {
    stream << ",\"kind\":\"stage\"";
  }
}

template <class Descriptor>
void append_graph_stage_json(std::ostringstream& stream, const Descriptor& descriptor,
                             const pb::runtime::descriptor_stage_record& stage) {
  stream << "{\"index\":" << stage.index << ",\"key\":";
  append_json_string(stream, stage.key);
  stream << ",\"name\":";
  append_json_string(stream, stage.name);
  append_stage_kind_json(stream, descriptor, stage);
  stream << "}";
}

template <class Descriptor>
void append_graph_stages_json(std::ostringstream& stream, const Descriptor& descriptor) {
  bool first_stage = true;
  for (const auto& stage : descriptor.stage_records()) {
    if (!first_stage) {
      stream << ",";
    }
    first_stage = false;
    append_graph_stage_json(stream, descriptor, stage);
  }
}

inline void append_graph_edge_json(std::ostringstream& stream, const pb::runtime::descriptor_edge_record& edge) {
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
  constexpr auto descriptor = pb::runtime::make_descriptor<Pipeline>();
  constexpr auto topology = decltype(descriptor)::topology == pb::runtime::descriptor_topology::branch
                                ? std::string_view{"branch"}
                                : std::string_view{"linear"};

  std::ostringstream stream;
  stream << "{\"schema_version\":\"pb.core.graph.v1\",\"topology\":";
  detail::append_json_string(stream, topology);
  stream << ",\"stage_count\":"
         << descriptor.stage_count << ",\"edge_count\":" << descriptor.edge_count << ",\"stages\":[";
  detail::append_graph_stages_json(stream, descriptor);
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
