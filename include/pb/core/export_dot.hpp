#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>

#include "pb/core/describe.hpp"

namespace pb::core {

namespace detail {

[[nodiscard]] inline auto sanitize_identifier(std::string_view value) -> std::string {
  std::string output(value.begin(), value.end());
  for (auto& ch : output) {
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_') {
      continue;
    }
    ch = '_';
  }
  if (output.empty()) {
    return "pipeline";
  }
  if ((output.front() >= '0' && output.front() <= '9')) {
    output.front() = '_';
  }
  return output;
}

} // namespace detail

template <ValidPipeline Pipeline>
[[nodiscard]] auto to_dot(std::string_view graph_name = "pipeline") -> std::string {
  constexpr auto descriptor = describe<Pipeline>();

  std::ostringstream stream;
  stream << "digraph " << detail::sanitize_identifier(graph_name) << " {\n";
  stream << "  rankdir=LR;\n";
  stream << "  node [shape=box];\n\n";

  for (std::size_t index = 0; index < descriptor.stage_count; ++index) {
    const auto key = descriptor.stage_keys()[index];
    const auto name = descriptor.stage_names()[index];
    stream << "  " << index << " [label=\"" << name;
    if (name != key) {
      stream << "\\n(" << key << ")";
    }
    stream << "\"]\n";
  }

  stream << "\n";

  for (const auto& edge : descriptor.edge_records()) {
    stream << "  " << edge.from_stage_index << " -> " << edge.to_stage_index;
    stream << " [label=\"" << edge.from_key << " -> " << edge.to_key << "\"]\n";
  }

  stream << "}\n";
  return stream.str();
}

} // namespace pb::core
