#include <pb/pipeline.hpp>

struct Input {};
struct Done {};

struct Always {
  using input_type = Input;
  using output_type = bool;
  bool operator()(const Input&) const { return true; }
};

struct VoidStage {
  using input_type = Input;
  using output_type = void;
  void operator()(Input) const {}
};

using Case = pb::case_<Always>::then<VoidStage>;

struct Join {
  using input_type = pb::fan_in_output_t<Case>;
  using output_type = Done;
  Done operator()(input_type) const { return {}; }
};

using Bad = pb::from<Input>::branch<Case>::fan_in<Join>::to<Done>;

int main() { return 0; }
