#include <pb/pipeline.hpp>

struct NotAStage {};

// Should fail with the public stage_input_t alias and missing input_type in diagnostics.
using BadInput = pb::stage_input_t<NotAStage>;

int main() { return 0; }
