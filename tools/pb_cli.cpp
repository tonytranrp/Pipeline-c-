// ─────────────────────────────────────────────────────────────
// Pipeline‑c++ CLI prototype  —  pb_cli
//
// A lightweight command‑line frontend for Pipeline‑c++
// introspection, export, and validation workflows.
//
// Usage:
//   pb_cli                        print banner & usage
//   pb_cli version                print version info
//   pb_cli features               print feature‑gate matrix
//   pb_cli export [--dot|--json]  print export format help
//   pb_cli validate <pipeline>    (reserved — future)
// ─────────────────────────────────────────────────────────────

#include <pb/pipeline.hpp>
#include <pb/core/cpp26_features.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

// ─────────────────────────────────────────────────────────────
// version constants
// ─────────────────────────────────────────────────────────────

static constexpr std::string_view kVersion    = "0.1.0";
static constexpr std::string_view kStdVersion  = "C++20";
static constexpr std::string_view kLibType     = "header-only";

// ─────────────────────────────────────────────────────────────
// helpers
// ─────────────────────────────────────────────────────────────

namespace {

[[nodiscard]] constexpr const char* yes_no(bool cond) noexcept {
  return cond ? "yes" : "no";
}

void print_banner() {
  std::cout << "Pipeline-c++ CLI v" << kVersion << "\n";
  std::cout << "Compile-time validated C++ pipeline builder\n\n";
  std::cout << "Usage: pb_cli <command> [options]\n\n";
  std::cout << "Commands:\n";
  std::cout << "  version              Print version and capability summary\n";
  std::cout << "  features             Print C++ feature‑gate matrix\n";
  std::cout << "  export [--dot|--json]  Print export format documentation\n";
  std::cout << "  validate <pipeline>  (reserved) Validate a pipeline definition\n";
}

void print_version() {
  std::cout << "Pipeline-c++ v" << kVersion << "\n";
  std::cout << "Standard:      " << kStdVersion << "\n";
  std::cout << "Distribution:  " << kLibType << "\n";
  std::cout << "Topologies:    linear, branch/join\n";
  std::cout << "Branch types:  homogeneous + heterogeneous (via std::variant)\n";
  std::cout << "Export:        DOT, JSON\n";
  std::cout << "Runtime:       sequential (built-in), thread‑pool (built‑in)\n";
}

void print_features() {
  std::cout << "Pipeline-c++ feature‑gate matrix\n";
  std::cout << "=================================\n";
  std::cout << "Compiler __cplusplus:         " << __cplusplus << "\n\n";

  std::cout << "C++26 core language:          " << yes_no(PB_HAS_CPP26)       << "\n";
  std::cout << "Static reflection  (P2996):   " << yes_no(PB_HAS_REFLECTION)   << "\n";
  std::cout << "Contracts          (P2900):   " << yes_no(PB_HAS_CONTRACTS)    << "\n";
  std::cout << "Pack indexing      (P2662):   " << yes_no(PB_HAS_PACK_INDEXING) << "\n";
  std::cout << "std::expected      (C++23):   " << yes_no(PB_HAS_STD_EXPECTED)  << "\n";
  std::cout << "Deducing this      (P0847):   " << yes_no(PB_HAS_DEDUCING_THIS) << "\n";

  std::cout << "\nFeature‑test macros (raw):\n";
#if defined(__cpp_reflection)
  std::cout << "  __cpp_reflection            = " << __cpp_reflection << "\n";
#else
  std::cout << "  __cpp_reflection            = <undefined>\n";
#endif
#if defined(__cpp_contracts)
  std::cout << "  __cpp_contracts             = " << __cpp_contracts << "\n";
#else
  std::cout << "  __cpp_contracts             = <undefined>\n";
#endif
#if defined(__cpp_pack_indexing)
  std::cout << "  __cpp_pack_indexing         = " << __cpp_pack_indexing << "\n";
#else
  std::cout << "  __cpp_pack_indexing         = <undefined>\n";
#endif
#if defined(__cpp_lib_expected)
  std::cout << "  __cpp_lib_expected          = " << __cpp_lib_expected << "\n";
#else
  std::cout << "  __cpp_lib_expected          = <undefined>\n";
#endif
#if defined(__cpp_explicit_this_parameter)
  std::cout << "  __cpp_explicit_this_parameter = " << __cpp_explicit_this_parameter << "\n";
#else
  std::cout << "  __cpp_explicit_this_parameter = <undefined>\n";
#endif
}

void print_export_help(std::string_view format) {
  if (format.empty() || format == "--dot") {
    std::cout << "DOT export (Graphviz)\n";
    std::cout << "=====================\n";
    std::cout << "Use pb::to_dot(pipeline) to produce a Graphviz .dot string.\n";
    std::cout << "Supported topologies: linear, branch/join.\n";
    std::cout << "Example:\n";
    std::cout << "  constexpr auto dot = pb::to_dot(my_pipeline);\n";
    std::cout << "  std::cout << dot.c_str();\n";
    if (format == "--dot") return;
    std::cout << "\n";
  }
  if (format.empty() || format == "--json") {
    std::cout << "JSON export\n";
    std::cout << "============\n";
    std::cout << "Use pb::to_json(pipeline) to produce a JSON representation.\n";
    std::cout << "Supported topologies: linear, branch/join.\n";
    std::cout << "Example:\n";
    std::cout << "  constexpr auto json = pb::to_json(my_pipeline);\n";
    std::cout << "  std::cout << json.c_str();\n";
  }
}

void print_validate_help() {
  std::cout << "validate — (reserved for future use)\n";
  std::cout << "Will accept a pipeline definition file and report\n";
  std::cout << "compile‑time validation results.\n";
}

} // anonymous namespace

// ─────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────

int main(int argc, char* argv[]) {
  if (argc < 2) {
    print_banner();
    return 0;
  }

  const std::string cmd = argv[1];

  if (cmd == "version") {
    print_version();
    return 0;
  }

  if (cmd == "features") {
    print_features();
    return 0;
  }

  if (cmd == "export") {
    std::string_view fmt;
    if (argc >= 3) fmt = argv[2];
    print_export_help(fmt);
    return 0;
  }

  if (cmd == "validate") {
    print_validate_help();
    return 0;
  }

  // ── unknown command ────────────────────────────────────────
  std::cerr << "Unknown command: " << cmd << "\n";
  std::cerr << "Run `pb_cli` without arguments for usage.\n";
  return 1;
}
