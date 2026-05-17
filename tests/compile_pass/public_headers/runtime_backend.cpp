#include <pb/runtime/backend.hpp>

#include <cstddef>
#include <type_traits>

static_assert(std::is_same_v<decltype(pb::runtime::backend_features().size()), std::size_t>);
static_assert(pb::runtime::backend_supported("sequential"));
static_assert(!pb::runtime::backend_supported("taskflow"));
static_assert(!pb::runtime::backend_supported("oneTBB"));
static_assert(!pb::runtime::backend_supported("stdexec"));

int pb_public_header_runtime_backend() { return 0; }
