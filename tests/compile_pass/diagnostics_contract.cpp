#include <pb/core/diagnostics.hpp>

#include <cassert>
#include <cstddef>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

// ── Schema version ────────────────────────────────────────────────────────

static_assert(pb::diagnostics::diagnostics_schema_version == "pb.diagnostics.v1",
              "Schema version must match pb.diagnostics.v1 for machine consumers");

// ── severity enum ─────────────────────────────────────────────────────────

static_assert(std::is_enum_v<pb::diagnostics::severity>,
              "severity must be an enum");
static_assert(sizeof(pb::diagnostics::severity) == sizeof(std::uint8_t),
              "severity must pack into std::uint8_t for stable serialisation");

// Verify each enumerator is distinct
static_assert(static_cast<int>(pb::diagnostics::severity::error) !=
                  static_cast<int>(pb::diagnostics::severity::warning),
              "error and warning must be distinct severity values");
static_assert(static_cast<int>(pb::diagnostics::severity::warning) !=
                  static_cast<int>(pb::diagnostics::severity::info),
              "warning and info must be distinct severity values");
static_assert(static_cast<int>(pb::diagnostics::severity::info) !=
                  static_cast<int>(pb::diagnostics::severity::suggestion),
              "info and suggestion must be distinct severity values");

// ── diagnostic_record — aggregate / default constructibility ──────────────

static_assert(std::is_default_constructible_v<pb::diagnostics::diagnostic_record>,
              "diagnostic_record must be default-constructible");
static_assert(std::is_copy_constructible_v<pb::diagnostics::diagnostic_record>,
              "diagnostic_record must be copyable for vector storage");
static_assert(std::is_move_constructible_v<pb::diagnostics::diagnostic_record>,
              "diagnostic_record must be movable");

// ── diagnostic_collector — basic operations ───────────────────────────────

static_assert(std::is_default_constructible_v<pb::diagnostics::diagnostic_collector>,
              "diagnostic_collector must be default-constructible");

// ── suggestions namespace ─────────────────────────────────────────────────

static_assert(!pb::diagnostics::suggestions::join_requires_branch.empty(),
              "join_requires_branch suggestion must be defined");
static_assert(!pb::diagnostics::suggestions::type_mismatch.empty(),
              "type_mismatch suggestion must be defined");

// Verify suggestions are string_view (lightweight, no allocation)
static_assert(std::is_same_v<decltype(pb::diagnostics::suggestions::join_requires_branch),
                             const std::string_view>,
              "suggestions must be const std::string_view");
static_assert(std::is_same_v<decltype(pb::diagnostics::suggestions::type_mismatch),
                             const std::string_view>,
              "suggestions must be const std::string_view");

// ── Runtime verification ──────────────────────────────────────────────────

int main() {
  // --- diagnostic_record designated-initializer construction ---
  // (std::string fields preclude constexpr, so we test at runtime)
  {
    pb::diagnostics::diagnostic_record rec{
        .level = pb::diagnostics::severity::error,
        .code = "PB-TEST-001",
        .message = "test diagnostic",
        .file = "diagnostics_contract.cpp",
        .line = 42,
        .column = 7,
        .stage_key = "parse",
        .suggestion = "Check input type",
    };

    assert(rec.level == pb::diagnostics::severity::error);
    assert(rec.code == "PB-TEST-001");
    assert(rec.message == "test diagnostic");
    assert(rec.file == "diagnostics_contract.cpp");
    assert(rec.line == 42);
    assert(rec.column == 7);
    assert(rec.stage_key == "parse");
    assert(rec.suggestion == "Check input type");
  }

  // --- collector::add with source_location ---
  pb::diagnostics::diagnostic_collector collector;
  collector.add(pb::diagnostics::severity::warning,
                "PB-WARN-001",
                "something looks suspicious");

  assert(collector.records.size() == 1);
  const auto& rec = collector.records[0];
  assert(rec.level == pb::diagnostics::severity::warning);
  assert(rec.code == "PB-WARN-001");
  assert(rec.message == "something looks suspicious");

  // source_location should capture this file's name
  assert(rec.file.find("diagnostics_contract.cpp") != std::string::npos);
  assert(rec.line > 0);    // line must be populated
  assert(rec.column > 0);  // column must be populated

  // Empty stage_key and suggestion by default
  assert(rec.stage_key.empty());
  assert(rec.suggestion.empty());

  // --- collector::add with stage_key and suggestion ---
  pb::diagnostics::diagnostic_collector collector2;
  {
    auto rec2 = pb::diagnostics::diagnostic_record{
        .level = pb::diagnostics::severity::error,
        .code = "PB-EDGE-001",
        .message = "Pipeline edge mismatch",
        .file = {},
        .line = {},
        .column = {},
        .stage_key = "normalize",
        .suggestion = std::string{pb::diagnostics::suggestions::type_mismatch},
    };
    collector2.records.push_back(std::move(rec2));
  }

  assert(collector2.records.size() == 1);
  const auto& rec2 = collector2.records[0];
  assert(rec2.level == pb::diagnostics::severity::error);
  assert(rec2.code == "PB-EDGE-001");
  assert(rec2.stage_key == "normalize");
  assert(rec2.suggestion == pb::diagnostics::suggestions::type_mismatch);

  // --- multiple records ---
  collector.add(pb::diagnostics::severity::info,
                "PB-INFO-001",
                "pipeline constructed successfully");
  collector.add(pb::diagnostics::severity::suggestion,
                "PB-SUGG-001",
                "consider adding a sink stage");

  assert(collector.records.size() == 3);

  return 0;
}
