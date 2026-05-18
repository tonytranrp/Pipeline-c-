#include <pb/pipeline.hpp>

#include <string_view>
#include <type_traits>

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

  static constexpr auto key = pb::fixed_string{"runtime.parse"};
  static constexpr auto name = pb::fixed_string{"parse"};
};

struct Finish {
  using input_type = Parsed;
  using output_type = Output;

  static constexpr auto key = pb::fixed_string{"runtime.finish"};
  static constexpr auto name = pb::fixed_string{"finish"};
};

using Pipeline = pb::from<Input>::then<Parse>::then<Finish>::to<Output>;
using EmptyPipeline = pb::from<Input>::to<Input>;

static_assert(pb::valid<Pipeline>);
static_assert(pb::descriptor_schema_version == std::string_view{"pb.descriptor.v1"});
static_assert(pb::descriptor_topology::linear == pb::runtime::descriptor_topology::linear);

constexpr auto descriptor = pb::make_descriptor<Pipeline>();
static_assert(decltype(descriptor)::stage_count == 2);
static_assert(decltype(descriptor)::edge_count == 1);
static_assert(!decltype(descriptor)::empty);
static_assert(decltype(descriptor)::schema_version == std::string_view{"pb.descriptor.v1"});
static_assert(decltype(descriptor)::topology == pb::descriptor_topology::linear);
static_assert(descriptor.stage_records()[0].index == 0);
static_assert(descriptor.stage_records()[0].key == std::string_view{"runtime.parse"});
static_assert(descriptor.stage_records()[1].name == std::string_view{"finish"});
static_assert(descriptor.edge_records()[0].from_key == std::string_view{"runtime.parse"});
static_assert(descriptor.edge_records()[0].to_name == std::string_view{"finish"});
static_assert(std::same_as<decltype(descriptor), const pb::runtime::descriptor_view<2>>);

constexpr auto empty_descriptor = pb::make_descriptor<EmptyPipeline>();
static_assert(decltype(empty_descriptor)::stage_count == 0);
static_assert(decltype(empty_descriptor)::edge_count == 0);
static_assert(decltype(empty_descriptor)::empty);
static_assert(empty_descriptor.stage_records().empty());
static_assert(empty_descriptor.edge_records().empty());

int pb_public_header_runtime_descriptor() { return 0; }
