#include "pb/pipeline.hpp"

struct s1 { using input_type = int; using output_type = int; };
struct s2 { using input_type = int; using output_type = int; };
struct s3 { using input_type = int; using output_type = int; };
struct s4 { using input_type = int; using output_type = int; };
struct s5 { using input_type = int; using output_type = int; };

using p1 = typename pb::from<int>::then<s1>::chain;
using p2 = typename p1::then<s2>::chain;
using p3 = typename p2::then<s3>::chain;
using p4 = typename p3::then<s4>::chain;
using pipeline = typename p4::then<s5>::chain;
static_assert(pb::core::is_valid_chain_v<pipeline>);

int main() { return 0; }
