#include <pb/pipeline.hpp>
#include <memory>

struct MoveOnly {
  MoveOnly() = default;
  MoveOnly(MoveOnly&&) = default;
  MoveOnly& operator=(MoveOnly&&) = default;
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;
  int value{};
};

struct Routed {
  int value{};
};

struct Always {
  using input_type = MoveOnly;
  using output_type = bool;
  bool operator()(const MoveOnly&) const { return true; }
};

struct Never {
  using input_type = MoveOnly;
  using output_type = bool;
  bool operator()(const MoveOnly&) const { return false; }
};

struct Double {
  using input_type = MoveOnly;
  using output_type = Routed;
  auto operator()(const MoveOnly& m) const -> Routed { return Routed{m.value * 2}; }
};

struct Triple {
  using input_type = MoveOnly;
  using output_type = Routed;
  auto operator()(const MoveOnly& m) const -> Routed { return Routed{m.value * 3}; }
};

using CaseA = pb::case_<Always>::then<Double>;
using CaseB = pb::case_<Never>::then<Triple>;

// selected_branch_node requires copy-constructible branch input
using BadPipeline = pb::from<MoveOnly>::branch<CaseA, CaseB>::to<Routed>;
static_assert(sizeof(BadPipeline) > 0);

int main() { return 0; }
