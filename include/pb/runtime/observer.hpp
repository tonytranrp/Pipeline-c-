#pragma once

#include <string_view>

#include "pb/runtime/error.hpp"

namespace pb::runtime {

/// Observer ABI version identifier.  Bumped only when the observer
/// vtable shape — method names, signatures, or count — changes in a
/// way that breaks ABI for derived observer implementations.  Adding
/// a new virtual method with a default no-op body is NOT a bump.
/// Removing a method, renaming one, changing a signature, or
/// reordering the vtable IS a bump.
///
/// Downstream consumers (custom observers) SHOULD pin against this
/// identifier in their build system or static_assert to detect drift.
inline constexpr std::string_view observer_schema_version = "pb.observer.v1";

/// Base observer for pipeline lifecycle events.
///
/// All virtual methods are non-pure with default no-op bodies so a
/// derived observer only overrides the events it cares about.  The
/// default no-op bodies are part of the v1 ABI promise — removing
/// them would require a v2 bump.
///
/// v1 event surface (locked):
///
///   on_stage_start     (stage_id)
///   on_stage_success   (stage_id)
///   on_stage_failure   (stage_id, error)
///   on_stage_exception (stage_id, error)
///   on_case_selected   (branch_id, case_index, predicate_id)
///   on_case_skipped    (branch_id, case_index, predicate_id)
///   on_case_failed     (branch_id, case_index, case_stage_id, diagnostic)
///
/// See docs/observer-schema-v1.md for the formal ABI contract.
struct observer {
  virtual ~observer() = default;

  virtual void on_stage_start(const stage_id&) {}
  virtual void on_stage_success(const stage_id&) {}
  virtual void on_stage_failure(const stage_id&, const error&) {}
  virtual void on_stage_exception(const stage_id&, const error&) {}
  virtual void on_case_selected(const stage_id& /*branch_id*/, std::size_t /*case_index*/,
                                const stage_id& /*predicate_id*/) {}
  virtual void on_case_skipped(const stage_id& /*branch_id*/, std::size_t /*case_index*/,
                               const stage_id& /*predicate_id*/) {}
  virtual void on_case_failed(const stage_id& /*branch_id*/, std::size_t /*case_index*/,
                              const stage_id& /*case_stage_id*/, const error& /*diagnostic*/) {}
};

/// Verbose event-line schema identifier.  The `[pb.verbose] <event>
/// <field>=<value>` line format produced by `pb::verbose_observer` is
/// a stable text contract.  Downstream log-parsing tools can rely on
/// the v1 line schema documented in docs/observer-schema-v1.md.
///
/// The schema is independent from the observer-ABI schema above: a
/// non-ABI-breaking event re-ordering (e.g. swapping field order on a
/// line) still bumps this constant.
inline constexpr std::string_view verbose_observer_schema_version = "pb.observer.verbose.v1";

} // namespace pb::runtime
