#pragma once

#include <string>
#include <string_view>
#include <utility>

#include "pb/core/describe.hpp"

namespace pb::core {

namespace detail {
inline auto escape_dot_label(std::string_view value) -> std::string {
  std::string escaped;
  escaped.reserve(value.size());

  for (const auto ch : value) {
    if (ch == '"' || ch == '\\') {
      escaped.push_back('\\');
    }
    escaped.push_back(ch);
  }

  return escaped;
}

inline auto graph_node_id(std::size_t stage_index) -> std::string {
  return "stage_" + std::to_string(stage_index);
}
} // namespace detail

template <ValidPipeline Pipeline>
[[nodiscard]] auto export_dot(std::string_view graph_name = "pipeline") -> std::string {
  constexpr auto desc = describe<Pipeline>();
  constexpr auto stages = desc.stage_count;
  constexpr auto edges = desc.edge_count;
  std::string graph{std::string{"digraph "} + std::string(graph_name) + " {\n"};
  graph += "  node [shape=box];\n";

  for (std::size_t index = 0; index < stages; ++index) {
    const auto stage_key = desc.stage_key(index);
    const auto stage_name = desc.stage_name(index);
    const auto& stage_key_ref = stage_key;
    const auto& stage_name_ref = stage_name;
    const auto node_id = detail::graph_node_id(index);
    const auto label = detail::escape_dot_label(std::string{stage_name_ref} + " (" + std::string{stage_key_ref} + ")");
    graph += "  " + node_id + " [label=\"" + label + "\"];\n";
  }

  for (std::size_t index = 0; index < edges; ++index) {
    const auto from = detail::graph_node_id(index);
    const auto to = detail::graph_node_id(index + 1);
    graph += "  " + from + " -> " + to + ";\n";
  }

  graph += "}\n";
  return graph;
}

} // namespace pb::core
