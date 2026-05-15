#include <pb/pipeline.hpp>

struct Input {
  int value{};
};

using Pipeline = pb::from<Input>::to<Input>;
static_assert(pb::valid<Pipeline>);

int main() {
  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  auto output = engine.run(Input{17});
  return output.value == 17 ? 0 : 1;
}
