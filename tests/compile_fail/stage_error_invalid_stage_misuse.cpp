#include <pb/pipeline.hpp>

struct NotAStage {};

// Should fail with the public stage_error_t alias and missing error_type in diagnostics.
using BadError = pb::stage_error_t<NotAStage>;

int main() { return 0; }
