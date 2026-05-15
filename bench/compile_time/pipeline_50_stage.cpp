#include <pb/pipeline.hpp>

template <int>
struct stage {
  using input_type = int;
  using output_type = int;
};

using pipeline_50 = pb::from<int>
    ::then<stage<0>>::to<stage<1>>
    ::then<stage<2>>::to<stage<3>>
    ::then<stage<4>>::to<stage<5>>
    ::then<stage<6>>::to<stage<7>>
    ::then<stage<8>>::to<stage<9>>
    ::then<stage<10>>::to<stage<11>>
    ::then<stage<12>>::to<stage<13>>
    ::then<stage<14>>::to<stage<15>>
    ::then<stage<16>>::to<stage<17>>
    ::then<stage<18>>::to<stage<19>>
    ::then<stage<20>>::to<stage<21>>
    ::then<stage<22>>::to<stage<23>>
    ::then<stage<24>>::to<stage<25>>
    ::then<stage<26>>::to<stage<27>>
    ::then<stage<28>>::to<stage<29>>
    ::then<stage<30>>::to<stage<31>>
    ::then<stage<32>>::to<stage<33>>
    ::then<stage<34>>::to<stage<35>>
    ::then<stage<36>>::to<stage<37>>
    ::then<stage<38>>::to<stage<39>>
    ::then<stage<40>>::to<stage<41>>
    ::then<stage<42>>::to<stage<43>>
    ::then<stage<44>>::to<stage<45>>
    ::then<stage<46>>::to<stage<47>>
    ::then<stage<48>>::to<stage<49>>;

static_assert(pb::core::validate<pipeline_50>::value);

int main() {
  return 0;
}
