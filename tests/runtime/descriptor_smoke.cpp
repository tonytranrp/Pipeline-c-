#include <pb/pipeline.hpp>

#include <cassert>
#include <string_view>

// ---------------------------------------------------------------------------
// Linear pipeline stages (existing)
// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------
// Branch pipeline stages
// ---------------------------------------------------------------------------

struct Document {
  int id{};
};

struct ProcessedDoc {
  int id{};
  int result{};
};

struct IsInvoice {
  using input_type = Document;
  using output_type = bool;

  static constexpr auto stage_name() noexcept { return "is-invoice"; }
  static constexpr auto stage_key() noexcept { return "is-invoice"; }

  bool operator()(const Document& doc) const { return doc.id == 1; }
};

struct IsReport {
  using input_type = Document;
  using output_type = bool;

  static constexpr auto stage_name() noexcept { return "is-report"; }
  static constexpr auto stage_key() noexcept { return "is-report"; }

  bool operator()(const Document& doc) const { return doc.id == 2; }
};

struct ProcessInvoice {
  using input_type = Document;
  using output_type = ProcessedDoc;

  static constexpr auto stage_name() noexcept { return "process-invoice"; }
  static constexpr auto stage_key() noexcept { return "process-invoice"; }

  ProcessedDoc operator()(const Document& doc) const { return ProcessedDoc{doc.id, doc.id * 10}; }
};

struct ProcessReport {
  using input_type = Document;
  using output_type = ProcessedDoc;

  static constexpr auto stage_name() noexcept { return "process-report"; }
  static constexpr auto stage_key() noexcept { return "process-report"; }

  ProcessedDoc operator()(const Document& doc) const { return ProcessedDoc{doc.id, doc.id * 20}; }
};

struct Finalize {
  using input_type = ProcessedDoc;
  using output_type = int;

  static constexpr auto stage_name() noexcept { return "finalize"; }
  static constexpr auto stage_key() noexcept { return "finalize"; }

  int operator()(const ProcessedDoc& doc) const { return doc.result; }
};

using InvoiceCase = pb::case_<IsInvoice>::then<ProcessInvoice>;
using ReportCase = pb::case_<IsReport>::then<ProcessReport>;

using BranchPipeline = pb::from<Document>::branch<InvoiceCase, ReportCase>::join<Finalize>::to<int>;
static_assert(pb::valid<BranchPipeline>);

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------

int main() {
  // =========================================================================
  // Section 1 — Existing linear descriptor tests (backward compatible)
  // =========================================================================

  constexpr auto descriptor = pb::make_descriptor<Pipeline>();
  constexpr auto core_descriptor = pb::core::descriptor_view<Pipeline>();

  // Schema version updated to pb.descriptor.v1
  static_assert(decltype(descriptor)::schema_version == std::string_view{"pb.descriptor.v1"});
  static_assert(decltype(descriptor)::topology == pb::descriptor_topology::linear);
  static_assert(decltype(descriptor)::stage_count == 2);
  static_assert(decltype(descriptor)::edge_count == 1);
  static_assert(decltype(descriptor)::case_count == 0);
  static_assert(!decltype(descriptor)::empty);
  static_assert(decltype(core_descriptor)::stage_count == decltype(descriptor)::stage_count);
  static_assert(decltype(core_descriptor)::edge_count == decltype(descriptor)::edge_count);

  // Stage records — existing index/key/name fields still work
  static_assert(descriptor.stage_records()[0].index == 0);
  static_assert(descriptor.stage_records()[0].key == std::string_view{"order.parse"});
  static_assert(descriptor.stage_records()[1].name == std::string_view{"finish"});

  // New fields on stage_record
  static_assert(descriptor.stage_records()[0].topology == pb::descriptor_topology::linear);
  static_assert(descriptor.stage_records()[1].topology == pb::descriptor_topology::linear);
  // input_type_name and output_type_name are populated (content varies by compiler)
  static_assert(!descriptor.stage_records()[0].input_type_name.empty());
  static_assert(!descriptor.stage_records()[0].output_type_name.empty());

  // Edge records — existing fields still work
  static_assert(descriptor.edge_records()[0].from_stage_index == 0);
  static_assert(descriptor.edge_records()[0].to_stage_index == 1);
  static_assert(descriptor.edge_records()[0].from_key == std::string_view{"order.parse"});
  static_assert(descriptor.edge_records()[0].to_key == std::string_view{"order.finish"});

  // Branch case records are empty for linear pipelines
  static_assert(descriptor.branch_case_records().empty());

  // Cross-check with core descriptor
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

  // Runtime engine descriptor
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

  // Empty pipeline
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

  // Runtime execution still works
  auto result = engine.run(Input{20});
  assert(result.value == 42);

  // =========================================================================
  // Section 2 — Branch descriptor tests (new)
  // =========================================================================

  constexpr auto branch_descriptor = pb::make_branch_descriptor<BranchPipeline>();

  // Schema version
  static_assert(branch_descriptor.schema_version == std::string_view{"pb.descriptor.v1"});

  // Topology is branch (because one of the stages is a branch node)
  static_assert(branch_descriptor.topology == pb::descriptor_topology::branch);

  // Branch pipeline has 2 stages: selected_branch_node + Finalize
  static_assert(branch_descriptor.stage_count == 2);
  static_assert(branch_descriptor.edge_count == 1);

  // Branch cases: InvoiceCase + ReportCase = 2
  static_assert(branch_descriptor.case_count == 2);
  static_assert(!branch_descriptor.empty);

  // First stage is the branch node — should have branch topology
  static_assert(branch_descriptor.stage_records()[0].topology == pb::descriptor_topology::branch);
  static_assert(branch_descriptor.stage_records()[0].key == std::string_view{"branch"});
  static_assert(branch_descriptor.stage_records()[0].name == std::string_view{"branch"});
  // Branch node has input_type_name and output_type_name populated
  static_assert(!branch_descriptor.stage_records()[0].input_type_name.empty());
  static_assert(!branch_descriptor.stage_records()[0].output_type_name.empty());

  // Second stage is Finalize — linear topology
  static_assert(branch_descriptor.stage_records()[1].topology == pb::descriptor_topology::linear);
  static_assert(branch_descriptor.stage_records()[1].key == std::string_view{"finalize"});
  static_assert(branch_descriptor.stage_records()[1].name == std::string_view{"finalize"});

  // Edge connects branch node (index 0) → Finalize (index 1)
  static_assert(branch_descriptor.edge_records()[0].from_stage_index == 0);
  static_assert(branch_descriptor.edge_records()[0].to_stage_index == 1);

  // Branch case records
  // Case 0: IsInvoice → ProcessInvoice
  if constexpr (branch_descriptor.case_count > 0) {
    static_assert(branch_descriptor.branch_case_records()[0].case_index == 0);
    static_assert(branch_descriptor.branch_case_records()[0].branch_stage_index == 0);
    static_assert(branch_descriptor.branch_case_records()[0].predicate_key == std::string_view{"is-invoice"});
    static_assert(branch_descriptor.branch_case_records()[0].predicate_name == std::string_view{"is-invoice"});
    static_assert(branch_descriptor.branch_case_records()[0].stage_key == std::string_view{"process-invoice"});
    static_assert(branch_descriptor.branch_case_records()[0].stage_name == std::string_view{"process-invoice"});
    static_assert(!branch_descriptor.branch_case_records()[0].input_type_name.empty());
    static_assert(!branch_descriptor.branch_case_records()[0].output_type_name.empty());

    // Case 1: IsReport → ProcessReport
    static_assert(branch_descriptor.branch_case_records()[1].case_index == 1);
    static_assert(branch_descriptor.branch_case_records()[1].branch_stage_index == 0);
    static_assert(branch_descriptor.branch_case_records()[1].predicate_key == std::string_view{"is-report"});
    static_assert(branch_descriptor.branch_case_records()[1].predicate_name == std::string_view{"is-report"});
    static_assert(branch_descriptor.branch_case_records()[1].stage_key == std::string_view{"process-report"});
    static_assert(branch_descriptor.branch_case_records()[1].stage_name == std::string_view{"process-report"});
  }

  // Runtime branch engine descriptor
  auto branch_engine = pb::compile<BranchPipeline>(pb::runtime::sequential{});
  const auto runtime_branch_descriptor = branch_engine.descriptor();
  assert(runtime_branch_descriptor.schema_version == pb::runtime::descriptor_schema_version);
  assert(runtime_branch_descriptor.topology == pb::descriptor_topology::branch);
  assert(runtime_branch_descriptor.stage_records().size() == 2);
  assert(runtime_branch_descriptor.branch_case_records().size() == 2);
  assert(runtime_branch_descriptor.branch_case_records()[0].branch_stage_index == 0);
  assert(runtime_branch_descriptor.branch_case_records()[1].stage_key == "process-report");

  // Runtime execution
  auto br_result = branch_engine.run(Document{1});
  assert(br_result.has_value());
  assert(br_result.value() == 10);  // Invoice: id=1 → 1*10 = 10

  auto br_result2 = branch_engine.run(Document{2});
  assert(br_result2.has_value());
  assert(br_result2.value() == 40); // Report: id=2 → 2*20 = 40

  // =========================================================================
  // Section 3 — type_name utility tests
  // =========================================================================

  // type_name<int> returns a non-empty string_view
  static_assert(!pb::type_name<int>().empty());

  // type_name is constexpr
  constexpr auto int_name = pb::type_name<int>();
  static_assert(!int_name.empty());

  return 0;
}
