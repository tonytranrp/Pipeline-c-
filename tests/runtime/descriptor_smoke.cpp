#include <pb/pipeline.hpp>

#include <cassert>
#include <string_view>

struct Input {
  int value{};
};

struct Parsed {
  int value{};
};

struct Output {
  int value{};
};

struct Parse {
  using input_type = Input;
  using output_type = Parsed;

  static constexpr auto stage_key() noexcept { return "order.parse"; }
  static constexpr auto stage_name() noexcept { return "parse"; }

  Parsed operator()(Input input) const { return Parsed{input.value + 1}; }
};

struct Finish {
  using input_type = Parsed;
  using output_type = Output;

  static constexpr auto stage_key() noexcept { return "order.finish"; }
  static constexpr auto stage_name() noexcept { return "finish"; }

  Output operator()(Parsed parsed) const { return Output{parsed.value * 2}; }
};

using Pipeline = pb::from<Input>::then<Parse>::then<Finish>::to<Output>;
static_assert(pb::valid<Pipeline>);

int main() {
  constexpr auto descriptor = pb::make_descriptor<Pipeline>();
  static_assert(decltype(descriptor)::schema_version == std::string_view{"pb.runtime.descriptor.v1"});
  static_assert(decltype(descriptor)::topology == pb::descriptor_topology::linear);
  static_assert(decltype(descriptor)::stage_count == 2);
  static_assert(decltype(descriptor)::edge_count == 1);
  static_assert(!decltype(descriptor)::empty);
  static_assert(descriptor.stage_records()[0].index == 0);
  static_assert(descriptor.stage_records()[0].key == std::string_view{"order.parse"});
  static_assert(descriptor.stage_records()[1].name == std::string_view{"finish"});
  static_assert(descriptor.edge_records()[0].from_stage_index == 0);
  static_assert(descriptor.edge_records()[0].to_stage_index == 1);
  static_assert(descriptor.edge_records()[0].from_key == std::string_view{"order.parse"});
  static_assert(descriptor.edge_records()[0].to_key == std::string_view{"order.finish"});

  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  const auto runtime_descriptor = engine.descriptor();
  assert(runtime_descriptor.stage_records().size() == 2);
  assert(runtime_descriptor.edge_records().size() == 1);
  assert(runtime_descriptor.stage_records()[0].name == "parse");
  assert(runtime_descriptor.edge_records()[0].to_name == "finish");

  auto result = engine.run(Input{20});
  assert(result.value == 42);

  return 0;
}
