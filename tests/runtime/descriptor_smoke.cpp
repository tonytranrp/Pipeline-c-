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

using EmptyPipeline = pb::from<Input>::to<Input>;

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
  constexpr auto core_descriptor = pb::descriptor_view<Pipeline>();
  static_assert(decltype(descriptor)::schema_version == std::string_view{"pb.runtime.descriptor.v1"});
  static_assert(decltype(descriptor)::topology == pb::descriptor_topology::linear);
  static_assert(decltype(descriptor)::stage_count == 2);
  static_assert(decltype(descriptor)::edge_count == 1);
  static_assert(!decltype(descriptor)::empty);
  static_assert(decltype(core_descriptor)::stage_count == decltype(descriptor)::stage_count);
  static_assert(decltype(core_descriptor)::edge_count == decltype(descriptor)::edge_count);
  static_assert(descriptor.stage_records()[0].index == 0);
  static_assert(descriptor.stage_records()[0].key == std::string_view{"order.parse"});
  static_assert(descriptor.stage_records()[1].name == std::string_view{"finish"});
  static_assert(descriptor.edge_records()[0].from_stage_index == 0);
  static_assert(descriptor.edge_records()[0].to_stage_index == 1);
  static_assert(descriptor.edge_records()[0].from_key == std::string_view{"order.parse"});
  static_assert(descriptor.edge_records()[0].to_key == std::string_view{"order.finish"});
  static_assert(core_descriptor.stage_records()[0].index == descriptor.stage_records()[0].index);
  static_assert(core_descriptor.stage_records()[0].key == descriptor.stage_records()[0].key);
  static_assert(core_descriptor.stage_records()[0].name == descriptor.stage_records()[0].name);
  static_assert(core_descriptor.stage_records()[1].index == descriptor.stage_records()[1].index);
  static_assert(core_descriptor.stage_records()[1].key == descriptor.stage_records()[1].key);
  static_assert(core_descriptor.stage_records()[1].name == descriptor.stage_records()[1].name);
  static_assert(core_descriptor.edge_records()[0].index == descriptor.edge_records()[0].index);
  static_assert(core_descriptor.edge_records()[0].from_stage_index == descriptor.edge_records()[0].from_stage_index);
  static_assert(core_descriptor.edge_records()[0].to_stage_index == descriptor.edge_records()[0].to_stage_index);
  static_assert(core_descriptor.edge_records()[0].from_key == descriptor.edge_records()[0].from_key);
  static_assert(core_descriptor.edge_records()[0].from_name == descriptor.edge_records()[0].from_name);
  static_assert(core_descriptor.edge_records()[0].to_key == descriptor.edge_records()[0].to_key);
  static_assert(core_descriptor.edge_records()[0].to_name == descriptor.edge_records()[0].to_name);

  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  const auto runtime_descriptor = engine.descriptor();
  assert(runtime_descriptor.schema_version == descriptor.schema_version);
  assert(runtime_descriptor.topology == descriptor.topology);
  assert(runtime_descriptor.stage_records().size() == descriptor.stage_records().size());
  assert(runtime_descriptor.edge_records().size() == descriptor.edge_records().size());
  assert(runtime_descriptor.stage_records().size() == 2);
  assert(runtime_descriptor.edge_records().size() == 1);
  assert(runtime_descriptor.stage_records()[0].index == descriptor.stage_records()[0].index);
  assert(runtime_descriptor.stage_records()[0].key == descriptor.stage_records()[0].key);
  assert(runtime_descriptor.stage_records()[0].name == descriptor.stage_records()[0].name);
  assert(runtime_descriptor.stage_records()[1].index == descriptor.stage_records()[1].index);
  assert(runtime_descriptor.stage_records()[1].key == descriptor.stage_records()[1].key);
  assert(runtime_descriptor.stage_records()[1].name == descriptor.stage_records()[1].name);
  assert(runtime_descriptor.edge_records()[0].index == descriptor.edge_records()[0].index);
  assert(runtime_descriptor.edge_records()[0].from_stage_index == descriptor.edge_records()[0].from_stage_index);
  assert(runtime_descriptor.edge_records()[0].to_stage_index == descriptor.edge_records()[0].to_stage_index);
  assert(runtime_descriptor.edge_records()[0].from_key == descriptor.edge_records()[0].from_key);
  assert(runtime_descriptor.edge_records()[0].from_name == descriptor.edge_records()[0].from_name);
  assert(runtime_descriptor.edge_records()[0].to_key == descriptor.edge_records()[0].to_key);
  assert(runtime_descriptor.edge_records()[0].to_name == descriptor.edge_records()[0].to_name);

  assert(runtime_descriptor.stage_records()[0].name == "parse");
  assert(runtime_descriptor.edge_records()[0].to_name == "finish");
  static_assert(std::same_as<decltype(runtime_descriptor), const pb::runtime::descriptor_view<2>>);

  auto empty_engine = pb::compile<EmptyPipeline>(pb::runtime::sequential{});
  const auto empty_runtime_descriptor = empty_engine.descriptor();
  constexpr auto empty_descriptor = pb::make_descriptor<EmptyPipeline>();
  static_assert(decltype(empty_runtime_descriptor)::stage_count == 0);
  static_assert(decltype(empty_runtime_descriptor)::edge_count == 0);
  static_assert(decltype(empty_runtime_descriptor)::empty);
  static_assert(empty_descriptor.schema_version == pb::runtime::descriptor_schema_version);
  assert(empty_runtime_descriptor.schema_version == empty_descriptor.schema_version);
  assert(empty_runtime_descriptor.topology == empty_descriptor.topology);
  assert(empty_runtime_descriptor.stage_records().empty());
  assert(empty_runtime_descriptor.edge_records().empty());

  auto result = engine.run(Input{20});
  assert(result.value == 42);

  return 0;
}
