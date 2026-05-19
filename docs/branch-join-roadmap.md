# Branch / Join Roadmap / Status

Sequential branch execution is now a **supported, tested feature** for homogeneous branch outputs with optional join stages. A first heterogeneous-output slice is also implemented: differing branch outputs are represented as `std::variant<...>` and can be joined either by a stage that accepts that variant or by a type-list join stage that declares `pb::meta::type_list<...>` and overloads every raw branch output type; duplicate output types are preserved and routed by variant index rather than by type. Move-only branch inputs are supported when predicates inspect by `const input_type&` and the selected branch stage consumes the value.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time metadata/introspection through `describe()` and stage records
- sequential runtime execution for validated linear pipelines
- **public branch/join DSL**: `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>`
- **compile-time join validation**: `pipeline_state::join` validates branch context (must follow `::branch<...>`, join stage input must match the unified branch execution output)
- **runtime branch execution**: predicate evaluation, first-match-wins case selection with short-circuit, branch-stage execution, observer events (`on_case_selected`, `on_case_skipped`), error annotation with `[branch]` prefix
- **branch child identities**: unnamed branch predicates/stages receive runtime fallback keys such as `branch.case.0.predicate` and `branch.case.0.stage` so observer/error events do not collapse to the parent branch stage index
- **stateful branch execution**: branch predicates and stages respect `pb::runtime::stateful_sequential` policy, preserving stage state across multiple runs
- **heterogeneous branch outputs**: selected branch nodes return `std::variant<Ts...>` when case outputs differ, preserve duplicate output alternatives by index, and join stages can consume either that variant or raw `pb::meta::type_list<...>` metadata through overload-based selected-output dispatch
- **move-only branch inputs**: predicates observe an lvalue reference and the selected branch stage receives the moved input; coverage includes `std::unique_ptr` ownership transfer into a by-value branch stage
- branch-aware export helpers: DOT includes branch/case structure and JSON now reports branch topology instead of always reporting linear topology
- branch marker aliases (`case_`, `branch_case`, `branch_node`, `join_node`) plus `branch_case_output` / `branch_outputs` marker metadata, raw output type-list helpers, unified branch output helpers, homogeneous branch-output validation, unified-output validation, branch source compatibility, predicate marker, homogeneous branch-node case-input, branch-output compatibility validation, join consumption validation, invalid join-stage, and branch-output marker misuse diagnostics
- runtime route descriptor and ordered `select_route(...)` helper
- supported branch/join examples (`branch_routing_demo.cpp`, `branch_error_handling.cpp`)
- comprehensive branch runtime tests (multi-case routing, predicate errors, predicate exceptions, stage exceptions, observer events, try_run with join, stateful, multiple runs)

## Branch output metadata vocabulary

`pb::branch_outputs<Cases...>` exposes two intentionally different output views:

- `output_types` / `pb::branch_raw_output_types_t<Cases...>` is raw metadata: a `pb::meta::type_list<...>` preserving every case-stage output type in case order.
- `output_type` / `pb::branch_unified_output_t<Cases...>` is the execution output: a single `T` for homogeneous branches or `std::variant<Ts...>` for heterogeneous branches.

`pb::branch_output_validation<Outputs, Output>` remains the homogeneous compatibility check that every raw case output is exactly `Output`. `pb::branch_unified_output_validation<Outputs, Output>` checks the execution output contract and should be used when validating a heterogeneous branch against its `std::variant` join input.

Today the repository does **not** yet support:

- parallel all-branches fan-in / backend multi-input join lowering beyond the current selected-output variant/type-list join model
- predicate patterns that require consuming a move-only input before routing; predicates must remain callable with `const input_type&`
- parallel all-branches fan-in / backend multi-input join execution, stable descriptor/export compatibility, or backend/parallel branch execution

## Why branch/join matters

Branch/join support is the main step from a linear pipeline DSL to a richer graph model. The research plan treats it as the feature that enables:

- parallel or conditional flow through multiple stage paths
- explicit join points after divergent processing
- richer graph export and execution backends
- diagnostics for multi-input and multi-output pipeline shapes

Without this feature, the current library remains intentionally limited to one validated chain of stages.

## Scope relative to the research plan

The research plan originally treated branch/join as post-MVP work after the linear chain. The current repository now ships the first production-readiness slice of that plan: homogeneous sequential branch/join execution with public DSL, validation, runtime routing, observer events, stateful storage, tests, and examples.

The broader graph-shaped plan is still not complete. Heterogeneous branch outputs and move-only branch inputs have first implementation slices, including duplicate-output variant routing, explicit raw-vs-unified output validation helpers, and negative coverage for move-only predicates that would consume by value. Parallel all-branches fan-in joins, stable descriptor/export compatibility, release-grade golden export schemas, and backend branch execution remain roadmap work.

## Non-goals for the current MVP

The current MVP should **not** claim:

- parallel/all-branches multi-input stage support before backend join lowering exists
- multi-output branch lowering for ordinary stage outputs
- feedback/cycle execution
- stable branch label syntax or join result policy

Those decisions belong to a later implementation slice with explicit tests, examples, and executor contracts.

## What is supported for branch/join

1. **Public graph-builder API** ✅  
   `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>` with compile-time validation.
2. **Compile-time validation coverage** ✅  
   Checks for branch predicates, join context, and type compatibility. Branch-output and join validation have accepted evidence.
3. **Runtime execution coverage** ✅  
   Sequential branch routing with first-match-wins short-circuit, observer events, error propagation, stateful storage, const-input predicate evaluation, join stage execution, variant joins and type-list selected-output joins for heterogeneous outputs including duplicate output alternatives by index, and move-only input consumption by the selected branch stage.
4. **Targeted tests and examples** ✅  
   Positive runtime tests, negative compile-fail diagnostics, branch export compile-pass tests, and user-facing examples.

## Remaining roadmap items

- parallel all-branches fan-in / true backend multi-input join execution
- Stable descriptor/export compatibility with schema docs and release-grade golden outputs
- Parallel/async backend support for branch execution
- Broader move-only edge coverage beyond const-reference predicates plus selected-stage consumption

## Verification status today

- compile-pass coverage including `branch_join_validation_compile_pass.cpp`
- compile-fail diagnostic coverage for join-without-branch, join-type-mismatch, and 15+ other branch/join boundary diagnostics
- runtime branch execution tests: `sequential_branch_smoke.cpp`, `sequential_branch_execution_smoke.cpp`, `sequential_branch_comprehensive.cpp`, `sequential_branch_heterogeneous.cpp` (including duplicate-output `std::variant` routing), and `sequential_branch_move_only.cpp`
- user-facing branch examples: `branch_routing_demo.cpp` (3-case document routing + join), `branch_error_handling.cpp` (error propagation)
- observer event coverage for branch case selection/skipping and unnamed predicate/stage fallback identities
- stateful branch storage tested via `sequential_branch_comprehensive.cpp`
- graph helper coverage: `pb_export_dot_branch_compile_pass`, `pb_export_json_compile_pass`, `pb_export_json_branch_compile_pass`, and helper-output regression coverage in `pb_export_golden_compile_pass`

Parallel all-branches fan-in, stable descriptor/export compatibility, and backend branch execution remain future slices.

## Release guidance

Release notes and docs should describe branch/join support as:

> sequential branch routing with optional join stages is supported for homogeneous outputs, first-slice heterogeneous outputs through `std::variant` or type-list selected-output joins (including duplicate output alternatives by index), and move-only selected-branch input consumption; parallel fan-in joins, stable descriptor/export compatibility, and backend branch execution remain roadmap work

If a future slice adds branch/join support, update this page together with:

- `docs/production-readiness.md`
- graph-export and observer/tracing docs when those surfaces begin to depend on graph topology
- the relevant tests/examples/bench entries that validate topology, diagnostics, and runtime behavior
