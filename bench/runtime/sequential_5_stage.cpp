#include <pb/pipeline.hpp>

template <int>
struct stage {
  using input_type = int;
  using output_type = int;
};

using pipeline_5 = pb::from<int>
    ::then<stage<0>>::to<stage<1>>
    ::then<stage<2>>::to<stage<3>>
    ::then<stage<4>>::chain;

static_assert(pb::core::validate<pipeline_5>::value);

int main() {
  return 0;
}
