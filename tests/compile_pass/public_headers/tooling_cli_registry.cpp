#include <pb/tooling/cli_registry.hpp>

#include <type_traits>

namespace {
static_assert(std::is_default_constructible_v<pb::tooling::pipeline_registry>);
static_assert(std::is_default_constructible_v<pb::tooling::pipeline_entry>);
} // namespace

int pb_public_header_tooling_cli_registry() {
  pb::tooling::pipeline_registry registry;
  return registry.size() == 0 ? 0 : 1;
}
