#pragma once

#include "pb/runtime/error.hpp"

namespace pb::runtime {

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

} // namespace pb::runtime
