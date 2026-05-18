#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <string_view>

#include "pb/core/describe.hpp"
#include "pb/core/pipeline_state.hpp"

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

// ── Compile-time branch detection ───────────────────────────────────────────

template <class StageList>
struct has_branch_stage_impl;

template <class... Stages>
struct has_branch_stage_impl<meta::type_list<Stages...>>
    : std::bool_constant<(is_selected_branch_node<Stages>::value || ...)> {};

template <class Pipeline>
inline constexpr bool pipeline_has_branch_v =
    has_branch_stage_impl<typename pipeline_traits<Pipeline>::stages>::value;

// ── Branch case DOT rendering ───────────────────────────────────────────────

template <class Case, std::size_t BranchIndex, std::size_t CaseIndex>
void append_branch_case_subgraph(std::ostringstream& stream) {
  using Predicate = typename Case::predicate_type;
  using BranchStage = typename Case::stage_type;

  const auto pred_name = stage_traits<Predicate>::name();
  const auto stage_name = stage_traits<BranchStage>::name();

  stream << "  subgraph cluster_case_" << BranchIndex << "_" << CaseIndex << " {\n";
  stream << "    label=\"Case " << CaseIndex << ": " << pred_name << "\";\n";
  stream << "    pred_" << BranchIndex << "_" << CaseIndex
         << " [label=\"pred: " << pred_name << "\"];\n";
  stream << "    case_" << BranchIndex << "_" << CaseIndex
         << " [label=\"" << stage_name << "\"];\n";
  stream << "    branch_" << BranchIndex << " -> pred_" << BranchIndex << "_" << CaseIndex
         << " [style=dashed, label=\"test\"];\n";
  stream << "    pred_" << BranchIndex << "_" << CaseIndex << " -> case_" << BranchIndex << "_"
         << CaseIndex << ";\n";
  stream << "  }\n";
}

template <class Cases, std::size_t BranchIndex, std::size_t... CaseIndexes>
void append_branch_case_subgraphs(std::ostringstream& stream,
                                  std::index_sequence<CaseIndexes...>) {
  ((append_branch_case_subgraph<meta::at_t<Cases, CaseIndexes>, BranchIndex, CaseIndexes>(stream)),
   ...);
}

template <class Stage, std::size_t StageIndex>
void append_branch_stage_dot(std::ostringstream& stream) {
  stream << "  branch_" << StageIndex << " [shape=diamond, label=\"branch\"];\n\n";

  using Cases = typename Stage::cases;
  append_branch_case_subgraphs<Cases, StageIndex>(
      stream, std::make_index_sequence<Stage::case_count>{});
  stream << "\n";
}

// ── Branch-aware DOT emitter ────────────────────────────────────────────────

template <class Pipeline, bool HasBranch>
struct dot_emitter;

// Linear pipeline emitter (unchanged behaviour)
template <class Pipeline>
struct dot_emitter<Pipeline, false> {
  [[nodiscard]] static auto emit(std::string_view graph_name) -> std::string {
    constexpr auto descriptor = describe<Pipeline>();

    std::ostringstream stream;
    stream << "digraph " << detail::sanitize_identifier(graph_name) << " {\n";
    stream << "  rankdir=LR;\n";
    stream << "  node [shape=box];\n\n";

    for (std::size_t index = 0; index < descriptor.stage_count; ++index) {
      const auto key = descriptor.stage_keys()[index];
      const auto name = descriptor.stage_names()[index];
      stream << "  stage_" << index << " [label=\"" << name;
      if (name != key) {
        stream << "\\n(" << key << ")";
      }
      stream << "\"];\n";
    }

    stream << "\n";

    for (const auto& edge : descriptor.edge_records()) {
      stream << "  stage_" << edge.from_stage_index << " -> stage_" << edge.to_stage_index;
      stream << " [label=\"" << edge.from_key << " -> " << edge.to_key << "\"];\n";
    }

    stream << "}\n";
    return stream.str();
  }
};

// Branch pipeline emitter
template <class Pipeline>
struct dot_emitter<Pipeline, true> {
  using traits = pipeline_traits<Pipeline>;
  using stages = typename traits::stages;
  static constexpr std::size_t stage_count = traits::stage_count;

  [[nodiscard]] static auto emit(std::string_view graph_name) -> std::string {
    std::ostringstream stream;
    stream << "digraph " << detail::sanitize_identifier(graph_name) << " {\n";
    stream << "  rankdir=LR;\n";
    stream << "  node [shape=box];\n\n";

    // ── Input node ──────────────────────────────────────────────
    stream << "  from_input [label=\"Input\"];\n\n";

    // ── Stage nodes ─────────────────────────────────────────────
    emit_stages(stream, std::make_index_sequence<stage_count>{});

    // ── Output node ─────────────────────────────────────────────
    stream << "  to_output [shape=doublecircle, label=\"Output\"];\n\n";

    // ── Edges ───────────────────────────────────────────────────
    emit_all_edges(stream);

    stream << "}\n";
    return stream.str();
  }

private:
  // ── Stage node emission ──────────────────────────────────────

  template <std::size_t... Indexes>
  static void emit_stages(std::ostringstream& stream, std::index_sequence<Indexes...>) {
    ((emit_single_stage<pipeline_stage_t<Pipeline, Indexes>, Indexes>(stream)), ...);
  }

  template <class Stage, std::size_t StageIndex>
  static void emit_single_stage(std::ostringstream& stream) {
    if constexpr (is_selected_branch_node<Stage>::value) {
      detail::append_branch_stage_dot<Stage, StageIndex>(stream);
    } else {
      constexpr auto desc = describe<Pipeline>();
      const auto key = desc.stage_keys()[StageIndex];
      const auto name = desc.stage_names()[StageIndex];
      stream << "  stage_" << StageIndex << " [label=\"" << name;
      if (name != key) {
        stream << "\\n(" << key << ")";
      }
      stream << "\"];\n";
    }
  }

  // ── Edge emission ────────────────────────────────────────────

  static void emit_all_edges(std::ostringstream& stream) {
    // Input -> first stage
    emit_input_to_first_edge(stream);

    // Stage-to-stage edges (for each adjacent pair)
    emit_adjacent_edges(stream, std::make_index_sequence<stage_count>{});

    // Last stage -> output
    emit_last_to_output_edge(stream);
  }

  static void emit_input_to_first_edge(std::ostringstream& stream) {
    if constexpr (stage_count > 0) {
      using FirstStage = pipeline_stage_t<Pipeline, 0>;
      if constexpr (is_selected_branch_node<FirstStage>::value) {
        stream << "  from_input -> branch_0;\n";
      } else {
        stream << "  from_input -> stage_0;\n";
      }
    } else {
      stream << "  from_input -> to_output;\n";
    }
  }

  template <std::size_t... Indexes>
  static void emit_adjacent_edges(std::ostringstream& stream,
                                  std::index_sequence<Indexes...>) {
    ((emit_adjacent_edge<Indexes>(stream)), ...);
  }

  template <std::size_t Index>
  static void emit_adjacent_edge(std::ostringstream& stream) {
    if constexpr (Index + 1 < stage_count) {
      using CurrentStage = pipeline_stage_t<Pipeline, Index>;
      using NextStage = pipeline_stage_t<Pipeline, Index + 1>;

      if constexpr (is_selected_branch_node<CurrentStage>::value) {
        // Branch -> next: connect all case output stages to next stage
        emit_branch_to_next_edges<CurrentStage, NextStage, Index>(stream);
      } else if constexpr (is_selected_branch_node<NextStage>::value) {
        // Previous -> branch diamond
        stream << "  stage_" << Index << " -> branch_" << (Index + 1) << ";\n";
      } else {
        // Regular -> regular
        stream << "  stage_" << Index << " -> stage_" << (Index + 1) << ";\n";
      }
    }
  }

  template <class BranchStage, class NextStage, std::size_t BranchIndex>
  static void emit_branch_to_next_edges(std::ostringstream& stream) {
    emit_branch_to_next_edges_impl<BranchStage, NextStage, BranchIndex>(
        stream, std::make_index_sequence<BranchStage::case_count>{});
  }

  template <class BranchStage, class NextStage, std::size_t BranchIndex,
            std::size_t... CaseIndexes>
  static void emit_branch_to_next_edges_impl(std::ostringstream& stream,
                                             std::index_sequence<CaseIndexes...>) {
    if constexpr (is_selected_branch_node<NextStage>::value) {
      ((stream << "  case_" << BranchIndex << "_" << CaseIndexes << " -> branch_"
               << (BranchIndex + 1) << ";\n"),
       ...);
    } else {
      ((stream << "  case_" << BranchIndex << "_" << CaseIndexes << " -> stage_"
               << (BranchIndex + 1) << ";\n"),
       ...);
    }
  }

  static void emit_last_to_output_edge(std::ostringstream& stream) {
    if constexpr (stage_count > 0) {
      using LastStage = pipeline_stage_t<Pipeline, stage_count - 1>;
      if constexpr (is_selected_branch_node<LastStage>::value) {
        emit_branch_to_output_edges<LastStage>(stream);
      } else {
        stream << "  stage_" << (stage_count - 1) << " -> to_output;\n";
      }
    }
  }

  template <class BranchStage>
  static void emit_branch_to_output_edges(std::ostringstream& stream) {
    emit_branch_to_output_edges_impl<BranchStage>(
        stream, std::make_index_sequence<BranchStage::case_count>{});
  }

  template <class BranchStage, std::size_t... CaseIndexes>
  static void emit_branch_to_output_edges_impl(std::ostringstream& stream,
                                               std::index_sequence<CaseIndexes...>) {
    ((stream << "  case_" << (stage_count - 1) << "_" << CaseIndexes
             << " -> to_output;\n"),
     ...);
  }
};

} // namespace detail

template <ValidPipeline Pipeline>
[[nodiscard]] auto to_dot(std::string_view graph_name = "pipeline") -> std::string {
  return detail::dot_emitter<Pipeline, detail::pipeline_has_branch_v<Pipeline>>::emit(graph_name);
}

} // namespace pb::core
