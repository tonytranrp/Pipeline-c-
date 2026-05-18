# Branch / Join Roadmap / Status

Sequential branch execution is now a **supported, tested feature** for homogeneous branch outputs with optional join stages. Heterogeneous branch/join with type-list/variant join inputs remains roadmap-only.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time metadata/introspection through `describe()` and stage records
- sequential runtime execution for validated linear pipelines
- **public branch/join DSL**: `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>`
- **compile-time join validation**: `pipeline_state::join` validates branch context (must follow `::branch<...>`, join stage input must match branch output)
- **runtime branch execution**: predicate evaluation, first-match-wins case selection with short-circuit, branch-stage execution, observer events (`on_case_selected`, `on_case_skipped`), error annotation with `[branch]` prefix
- **stateful branch execution**: branch predicates and stages respect `pb::runtime::stateful_sequential` policy, preserving stage state across multiple runs
- branch marker aliases (`case_`, `branch_case`, `branch_node`, `join_node`) plus `branch_case_output` / `branch_outputs` marker metadata, branch source compatibility, predicate marker, homogeneous branch-node case-input, branch-output compatibility validation, join consumption validation, invalid join-stage, and branch-output marker misuse diagnostics
- runtime route descriptor and ordered `select_route(...)` helper
- supported branch/join examples (`branch_routing_demo.cpp`, `branch_error_handling.cpp`)
- comprehensive branch runtime tests (multi-case routing, predicate errors, predicate exceptions, stage exceptions, observer events, try_run with join, stateful, multiple runs)

Today the repository does **not** yet support:

- heterogeneous branch outputs joined through `type_list` or `variant` join inputs (all branch cases must produce the same output type)
- move-only branch inputs (branch input must be copy-constructible so predicates and the selected case can inspect the same value)
- multi-input join execution, full graph export, or JSON export

## Why branch/join matters

Branch/join support is the main step from a linear pipeline DSL to a richer graph model. The research plan treats it as the feature that enables:

- parallel or conditional flow through multiple stage paths
- explicit join points after divergent processing
- richer graph export and execution backends
- diagnostics for multi-input and multi-output pipeline shapes

Without this feature, the current library remains intentionally limited to one validated chain of stages.

## Intended scope relative to the research plan

The research plan treats branch/join as planned work, not current behavior:

- branch and join modeling is explicitly deferred until after the MVP linear chain
- a future branch graph builder slice introduces `fork`, `select`, `join`, and branch labels
- current validation work covers branch predicate checks, branch output compatibility, join compatibility, and branch diagnostics
- future runtime work covers sequential branch execution before more advanced backends

The current repository documents the intent and ships validation-only pieces, but it does not yet ship the public graph-builder API or execution model needed to claim executable branch/join support.

## Non-goals for the current MVP

The current MVP should **not** claim:

- multi-input stage support before join support exists
- multi-output branch lowering for ordinary stage outputs
- feedback/cycle execution
- stable branch label syntax or join result policy

Those decisions belong to a later implementation slice with explicit tests, examples, and executor contracts.

## What is supported for homogeneous branch/join

1. **Public graph-builder API** ✅  
   `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>` with compile-time validation.
2. **Compile-time validation coverage** ✅  
   Checks for branch predicates, join context, and type compatibility. Branch-output and join validation have accepted evidence.
3. **Runtime execution coverage** ✅  
   Sequential branch routing with first-match-wins short-circuit, observer events, error propagation, stateful storage, and join stage execution.
4. **Targeted tests and examples** ✅  
   Positive runtime tests (3 files), negative compile-fail diagnostics (17+ tests), and user-facing examples (2 files).

## Remaining roadmap items

- Heterogeneous branch outputs with `type_list`/`variant` join inputs
- Move-only branch input support
- Full graph export (DOT/JSON)
- Parallel/async backend support for branch execution

## Verification status today

- compile-pass coverage including `branch_join_validation_compile_pass.cpp`
- compile-fail diagnostic coverage for join-without-branch, join-type-mismatch, and 15+ other branch/join boundary diagnostics
- runtime branch execution tests: `sequential_branch_smoke.cpp`, `sequential_branch_execution_smoke.cpp`, `sequential_branch_comprehensive.cpp`
- user-facing branch examples: `branch_routing_demo.cpp` (3-case document routing + join), `branch_error_handling.cpp` (error propagation)
- observer event coverage for branch case selection/skipping
- stateful branch storage tested via `sequential_branch_comprehensive.cpp`

Heterogeneous branch output joins, move-only branch inputs, and full graph export remain as future slices.

## Release guidance

Release notes and docs should describe branch/join support as:

> sequential branch routing with homogeneous branch outputs and optional join stages is supported; heterogeneous branch/join with type-list/variant join inputs, move-only branch inputs, and full graph export remain roadmap work

If a future slice adds branch/join support, update this page together with:

- `docs/production-readiness.md`
- graph-export and observer/tracing docs when those surfaces begin to depend on graph topology
- the relevant tests/examples/bench entries that validate topology, diagnostics, and runtime behavior
