#pragma once

#include "pb/runtime/error.hpp"

namespace pb::runtime {

struct observer {
  virtual ~observer() = default;

  virtual void on_stage_start(const stage_id&) {}
  virtual void on_stage_success(const stage_id&) {}
  virtual void on_stage_failure(const stage_id&, const error&) {}
  virtual void on_stage_exception(const stage_id&, const error&) {}
};

} // namespace pb::runtime
