#include <pb/pipeline.hpp>

struct NotAStage {};

// Should fail with the public stage_output_t alias and missing output_type in diagnostics.
using BadOutput = pb::stage_output_t<NotAStage>;

int main() { return 0; }
