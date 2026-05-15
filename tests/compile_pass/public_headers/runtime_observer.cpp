#include <pb/runtime/observer.hpp>

struct test_observer final : pb::runtime::observer {
  void on_stage_start(const pb::runtime::stage_id&) override {}
};

int pb_public_header_runtime_observer() {
  (void)static_cast<pb::runtime::observer*>(nullptr);
  return 0;
}
