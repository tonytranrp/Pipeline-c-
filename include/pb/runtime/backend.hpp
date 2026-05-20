#pragma once

#include <array>
#include <span>
#include <string_view>

namespace pb::runtime {

enum class backend_support {
  supported,
  roadmap,
  experimental,
};

enum class backend_execution_model {
  sequential,
  thread_pool,
  task_graph,
  filter_pipeline,
  sender_receiver,
};

struct backend_feature {
  std::string_view name;
  backend_execution_model execution_model;
  backend_support support;
  bool external_dependency;
  bool default_build;
  std::string_view note;
};

inline constexpr auto backend_feature_matrix = std::array{
    backend_feature{.name = "sequential",
                    .execution_model = backend_execution_model::sequential,
                    .support = backend_support::supported,
                    .external_dependency = false,
                    .default_build = true,
                    .note = "standard-library deterministic linear runtime"},
    backend_feature{.name = "thread_pool",
                    .execution_model = backend_execution_model::thread_pool,
                    .support = backend_support::supported,
                    .external_dependency = false,
                    .default_build = false,
                    .note = "standard-library thread-pool backend for supported fan-in pipeline execution"},
    backend_feature{.name = "taskflow",
                    .execution_model = backend_execution_model::task_graph,
                    .support = backend_support::roadmap,
                    .external_dependency = true,
                    .default_build = false,
                    .note = "planned optional adapter target; dependency is not required by core/runtime"},
    backend_feature{.name = "oneTBB",
                    .execution_model = backend_execution_model::filter_pipeline,
                    .support = backend_support::roadmap,
                    .external_dependency = true,
                    .default_build = false,
                    .note = "planned optional adapter target; dependency is not required by core/runtime"},
    backend_feature{.name = "stdexec",
                    .execution_model = backend_execution_model::sender_receiver,
                    .support = backend_support::experimental,
                    .external_dependency = true,
                    .default_build = false,
                    .note = "future sender/receiver adapter gated on compiler and library maturity"},
};

[[nodiscard]] constexpr auto backend_features() noexcept -> std::span<const backend_feature>;

[[nodiscard]] constexpr auto backend_features() noexcept -> std::span<const backend_feature> {
  return {backend_feature_matrix.data(), backend_feature_matrix.size()};
}

[[nodiscard]] constexpr auto backend_supported(std::string_view name) noexcept -> bool {
  for (const auto& feature : backend_feature_matrix) {
    if (feature.name == name) {
      return feature.support == backend_support::supported;
    }
  }
  return false;
}

} // namespace pb::runtime
