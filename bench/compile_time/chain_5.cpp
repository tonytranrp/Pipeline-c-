#include <pb/pipeline.hpp>

struct value { int n{}; };
struct s1 { using input_type = value; using output_type = value; value operator()(value v) const { return {v.n + 1}; } };
struct s2 { using input_type = value; using output_type = value; value operator()(value v) const { return {v.n + 1}; } };
struct s3 { using input_type = value; using output_type = value; value operator()(value v) const { return {v.n + 1}; } };
struct s4 { using input_type = value; using output_type = value; value operator()(value v) const { return {v.n + 1}; } };
struct s5 { using input_type = value; using output_type = value; value operator()(value v) const { return {v.n + 1}; } };
using pipeline = pb::from<value>::then<s1>::then<s2>::then<s3>::then<s4>::then<s5>::to<value>;
static_assert(pb::valid<pipeline>);
int main() { return 0; }
