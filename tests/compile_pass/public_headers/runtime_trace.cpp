#include <pb/runtime/trace.hpp>

int pb_public_header_runtime_trace() {
  pb::runtime::trace_recorder recorder{};
  pb::runtime::trace_observer observer{recorder};
  (void)observer.get_sink();
  return recorder.empty() ? 0 : 1;
}
