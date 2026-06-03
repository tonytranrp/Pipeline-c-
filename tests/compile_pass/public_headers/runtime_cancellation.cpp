#include <pb/runtime/cancellation.hpp>

int pb_public_header_runtime_cancellation() {
  pb::cancellation_source source;
  pb::cancellation_token token = source.token();
  (void)token.can_be_cancelled();
  (void)token.stop_requested();
  source.request_stop();
  (void)source.stop_requested();
  (void)pb::cancellation_schema_version;
  // A default-constructed token never reports a stop.
  pb::cancellation_token defaulted{};
  return (defaulted.can_be_cancelled() || defaulted.stop_requested()) ? 1 : 0;
}
