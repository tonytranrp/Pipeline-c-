#include <pb/pipeline.hpp>

#include <algorithm>
#include <cassert>
#include <string_view>

int main() {
  const auto features = pb::backend_features();
  assert(features.size() == pb::backend_feature_matrix.size());

  const auto named = [&](std::string_view name) {
    return std::find_if(features.begin(), features.end(), [&](const pb::backend_feature& feature) {
      return feature.name == name;
    });
  };

  const auto sequential = named("sequential");
  assert(sequential != features.end());
  assert(sequential->support == pb::backend_support::supported);
  assert(sequential->execution_model == pb::backend_execution_model::sequential);
  assert(!sequential->external_dependency);
  assert(sequential->default_build);
  assert(pb::backend_supported("sequential"));

  for (const auto backend : {"thread_pool", "taskflow", "oneTBB", "stdexec"}) {
    const auto feature = named(backend);
    assert(feature != features.end());
    assert(!feature->default_build);
    assert(!pb::backend_supported(backend));
  }

  assert(!pb::backend_supported("unknown"));
}
