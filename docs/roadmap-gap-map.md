# Roadmap Gap Map

This page maps the major themes in `research/pipeline_builder_cpp_research_plan.md` to the repository evidence that exists **today**. Use it to separate shipped MVP support from roadmap-only gaps, and to identify the next safe documentation or implementation slices without overstating production readiness.


For a compact release-governance table that maps each gap to evidence, tests, release status, and the next required slice, see [Research Verification Matrix](research-verification-matrix.md).
## Current audited baseline

Historical local audit baseline: `aa3b8f6` on `main`. That MVP audit found the linear pipeline builder sound and locally green: developer configure/build/CTest completed with **78/78 tests passed**. Later branch hardening updates promoted sequential branch execution into the supported surface, so use current candidate verification rather than the old audit when making branch/join claims.

This baseline supports planning and documentation claims only for the linear MVP. Any later code change should refresh the evidence before release-facing wording is updated.

## Reading this map

Each theme below records four things:

- **Current repository evidence** — what the tree already proves or documents.
- **Current support level** — shipped MVP support versus roadmap-only status.
- **Proof points** — the tests, examples, or docs that currently back the claim.
- **Safe next slice** — a narrow follow-on step that can strengthen the evidence without pretending the broader roadmap is complete.

## Theme map

### 1. Linear pipeline core

- **Current repository evidence:** The public docs and headers describe a linear typed pipeline builder with explicit stage metadata and compile-time validation.
- **Current support level:** **Shipped MVP support.**
- **Proof points:**
  - `docs/production-readiness.md`
  - `docs/examples.md`
  - `include/pb/core/describe.hpp`
  - compile-pass coverage under `tests/compile_pass/`
- **Safe next slice:** keep public docs synchronized with any future core API additions rather than broadening the supported topology claims.

### 2. Sequential runtime execution

- **Current repository evidence:** The repository documents and ships the sequential runtime path for validated linear pipelines.
- **Current support level:** **Shipped MVP support.**
- **Proof points:**
  - `docs/production-readiness.md`
  - `include/pb/runtime/sequential.hpp`
  - runtime smoke coverage under `tests/runtime/`
  - benchmark smoke references in `docs/build.md` and `docs/benchmark-workflow.md`
- **Safe next slice:** keep runtime behavior, docs, and smoke coverage aligned while known `run()` / `try_run()` parity and exception-handling hardening work remains explicit.
- **Current policy note:** exception capture is documented as split behavior today (`run()` versus `try_run()`), so roadmap-facing claims should avoid wording that implies unified exception-policy semantics.
- **Additional proof point for the policy note:** `docs/error-model-tutorial.md` explicitly documents the split and its current limits.

### 3. Diagnostics and compile-fail guidance

- **Current repository evidence:** Diagnostics are documented as partially supported for the linear MVP, with both compile-fail and runtime error-formatting coverage.
- **Current support level:** **Partially shipped MVP support with broader roadmap work still open.**
- **Proof points:**
  - `docs/diagnostics-roadmap.md`
  - `docs/diagnostic-example-walkthrough.md`
  - `examples/error_diagnostic.cpp`
  - `tests/run_compile_fail.cmake`
  - `tests/runtime/error_diagnostic_smoke.cpp`
- **Safe next slice:** add focused docs/test evidence only when it directly matches currently supported diagnostic behavior, and keep richer wording/schema guarantees labeled as future work.

### 4. Package-consumer and release-readiness gates

- **Current repository evidence:** The docs now describe the package-consumer contract and the exact release-readiness command set that should be collected on a candidate SHA.
- **Current support level:** **Shipped release-scaffolding support, not a claim of production completeness.**
- **Proof points:**
  - `docs/production-readiness.md`
  - `docs/release-readiness-checklist.md`
  - `docs/build.md`
  - `continue.md` historical checkpoint guidance
- **Safe next slice:** refresh candidate-specific evidence on the chosen release SHA and record blockers instead of converting scaffolding into production claims.

### 5. Branch/join topology

- **Current repository evidence:** Public branch/join DSL with compile-time validation, runtime sequential branch execution (first-match-wins with short-circuit), observer events (`on_case_selected`, `on_case_skipped`, `on_case_failed`), stateful storage, join stages, error propagation, heterogeneous outputs through `std::variant`, selected-output type-list joins, explicit sequential fan-in with skipped/completed/failed slots, void-output cases, borrowed move-only fan-in, move-only selected-branch input consumption, first thread-pool fan-in backend scheduling, and comprehensive tests and examples. For non-copyable fan-in inputs, `pb::shared_view<T>` provides a copy-cheap shared-ptr-backed wrapper so each case receives its own wrapper copy without cloning the owned value. `pb::projected<From, Projection, Stage>` is the projection adapter that wraps an existing stage with a user-supplied projection callable, enabling fan-in branches and linear chains to plug arbitrary extract/borrow strategies into stages that expect a narrower input type.
- **Current support level:** **Supported for homogeneous outputs, heterogeneous outputs through `std::variant`, selected-output type-list joins, explicit sequential fan-in, thread-pool fan-in scheduling, borrowed move-only fan-in, move-only selected-branch input consumption, `pb::shared_view<T>` non-copyable fan-in inputs, and `pb::projected` projection adapters.**
- **Proof points:**
  - `docs/branch-join-roadmap.md`
  - `include/pb/core/pipeline_state.hpp` (public `::branch<...>::join<...>` DSL)
  - `include/pb/runtime/sequential.hpp` (runtime branch routing with storage)
  - `include/pb/runtime/clone.hpp` (`pb::shared_view<T>`, `pb::projected`, `pb::fan_in_uses_copy_v`, `pb::fan_in_uses_borrow_v`)
  - `tests/runtime/sequential_branch_comprehensive.cpp`
  - `tests/runtime/fan_in_shared_view_smoke.cpp`
  - `tests/runtime/projected_stage_smoke.cpp`
  - `examples/branch_routing_demo.cpp`, `examples/branch_error_handling.cpp`
- **Safe next slice:** keep preemptive cancellation/scheduling policy, broader move-only ownership policies, dependency backend fan-in, and broad backend branch execution as separate design/implementation phases.

### 6. Graph export

- **Current repository evidence:** DOT/JSON helper slices exist for linear and supported branch pipelines, including branch topology detection in JSON. A `pb::export_text` text-format helper (`include/pb/core/export_text.hpp`) now ships alongside the JSON/DOT helpers. The export schema is backed by a typed contract (`docs/export-schema-v1.md`, `tests/compile_pass/schema_v1_contract.cpp`, `tests/compile_pass/schema_v1_extended_golden.cpp`) that pins the literal `"pb.core.graph.v1"` schema-version string and the field layout against byte-equal regression tests.
- **Current support level:** **Partial descriptor-record-backed export helper support including `pb::export_text`; the `pb.core.graph.v1` typed contract governs the current helper output. The contract is typed, not yet a promised frozen external interchange schema — field additions are permitted without bumping the major, but breaking changes require a version bump to `pb.core.graph.v2`.**
- **Proof points:**
  - `docs/graph-export-roadmap.md`
  - `docs/export-schema-v1.md` (formal v1 schema specification)
  - `docs/export-helper-schema.md`
  - `include/pb/core/export_text.hpp`
  - `tests/compile_pass/schema_v1_contract.cpp`
  - `tests/compile_pass/schema_v1_extended_golden.cpp`
  - `tests/compile_pass/export_text.cpp`, `tests/compile_pass/export_text_golden.cpp`
  - `tests/compile_pass/public_headers/export_text.cpp`
- **Safe next slice:** keep helper output aligned with the v1 contract; claim a frozen external schema only after stability promises, CLI/file export, and schema migration policy are established.
- **Coordination note:** the DOT/JSON/text helper APIs are narrower than full graph export. Release notes must distinguish them from a stable frozen graph schema, CLI/file export, and backend graph-execution claims.

### 7. Observer hooks

- **Current repository evidence:** Runtime observer callbacks are shipped for the sequential executor (`set_observer`, `observer`) with runtime smoke tests.
- **Current support level:** **Partially shipped MVP support (sequential runtime).**
- **Proof points:**
  - `docs/observer-hooks-roadmap.md`
  - `tests/runtime/sequential_observer_smoke.cpp`
  - `tests/runtime/sequential_observer_accessor_smoke.cpp`
  - `tests/runtime/sequential_observer_try_run_exception_smoke.cpp`
  - `tests/runtime/sequential_custom_error_observer_smoke.cpp`
- **Safe next slice:** add observer examples/benchmarks and harden public contract details (ABI/stability, lifecycle, ordering) before claiming full support.

### 8. Optional backends

- **Current repository evidence:** The repository explicitly limits current execution support to the standard-library sequential path and marks alternative backends as future work.
- **Current support level:** **Roadmap-only gap beyond sequential execution.**
- **Proof points:**
  - `docs/optional-backends-roadmap.md`
  - `docs/production-readiness.md`
  - `docs/build.md` notes that backend flags remain intentionally off in the base scaffold
- **Safe next slice:** keep dependency/runtime-backend claims off the supported surface until adapter targets, tests, and performance evidence exist.

### 9. Stable runtime descriptor

- **Current repository evidence:** A narrow linear runtime-descriptor helper exists today (`pb::make_descriptor<Pipeline>()` and `engine.descriptor()`), alongside compile-time introspection and runtime diagnostic stage-identity checks. The `pb.core.graph.v1` schema-version string is now backed by a typed contract (`tests/compile_pass/schema_v1_contract.cpp`) that enforces the literal identifier at compile time, giving the descriptor/export surface a formal versioned anchor for the first time.
- **Current support level:** **Partially shipped linear metadata support with a typed `pb.core.graph.v1` version contract; stable external export/compatibility guarantees (frozen schema, versioned migration path, CLI/file export) remain roadmap-only.**
- **Proof points:**
  - `docs/runtime-descriptor-roadmap.md`
  - `docs/export-schema-v1.md`
  - `docs/production-readiness.md`
  - `include/pb/core/describe.hpp`
  - `include/pb/runtime/descriptor.hpp`
  - `tests/compile_pass/public_headers/runtime_descriptor.cpp`
  - `tests/runtime/descriptor_smoke.cpp`
  - `tests/compile_pass/schema_v1_contract.cpp`
  - `tests/compile_pass/schema_v1_extended_golden.cpp`
- **Safe next slice:** keep the current linear helper documented and tested at its real boundary; treat the v1 typed contract as a regression net rather than a frozen external promise until schema migration policy is documented.

### 10. Benchmark evidence and performance budgets

- **Current repository evidence:** Benchmark lanes are documented as smoke/profiling scaffolding, not release thresholds.
- **Current support level:** **Shipped smoke scaffolding with roadmap-only performance governance.**
- **Proof points:**
  - `docs/build.md`
  - `docs/benchmark-workflow.md`
  - `docs/release-readiness-checklist.md`
- **Safe next slice:** collect reproducible benchmark evidence with context, but keep thresholds/regression budgets labeled as future work until explicitly established.

## Active missing-feature priority queue

This queue is the durable docs-lane view of the current missing-feature push. It is not a release claim: move an item to “supported” only after the implementation commit is integrated, tests pass on the candidate branch, and release-facing docs/examples are updated.

| Priority | Missing feature / evidence gap | Current docs-lane status | Promotion gate |
| --- | --- | --- | --- |
| 1 | `run()` / `try_run()` error-policy hardening and exception behavior evidence | **DSL shipped and parity-proven.** Five named wrappers are factory-built and engine-tested: `pb::with_throw_on_error` → `throwing_engine`, `pb::with_terminate_on_error` → `terminating_engine`, `pb::with_ignore_errors` → `ignoring_engine`, `pb::with_propagate_exceptions` → `propagating_engine`, `pb::with_verbose_diagnostics` → `verbose_engine` (owns a `pb::verbose_observer`). Cross-wrapper `run()` / `try_run()` parity is proven across linear, selected-output branch, and explicit fan-in topologies — every (wrapper, method, case) cell over success / declared `result<>` failure / thrown exception — via the parity matrix lane: `tests/runtime/policy_dsl_parity_matrix_linear.cpp`, `tests/runtime/policy_dsl_parity_matrix_branch.cpp`, `tests/runtime/policy_dsl_parity_matrix_fan_in.cpp`. Composability is shipped (`tests/runtime/policy_dsl_composability_smoke.cpp`): `verbose_engine` can wrap any of the other four wrappers because all five expose `set_observer` / `get_observer` / `describe` / `descriptor` directly on the wrapper surface. `std::expected`-shape stages flow through every wrapper with `error_category::expected_error` normalization (`tests/runtime/policy_dsl_std_expected_matrix.cpp`). | Promote when these surfaces are reflected in release-notes wording. Remaining work belongs to a separate gap (runtime-enforced `::with<pb::policy::...>` DSL beyond marker bundles). |
| 2 | `std::expected` / expected-like result policy evidence | Expected-like support exists, but direct standard-library matrix coverage remains a release-hardening gap. | Compile/runtime tests on the supported compiler/library matrix, or explicit fallback wording when `std::expected` is unavailable. |
| 3 | Compile-time benchmark baselines for 5-stage and 50-stage chains | Benchmark smoke scaffolding exists; baseline evidence and thresholds remain separate. | Recorded baseline artifacts with preset/compiler/commit context. |
| 4 | Explicit stateful stage storage policy | Narrow public guarantee exists for default-initializable sequential stages: per-run construction versus engine-stored state, including non-copyable owned stage state, is covered by policy aliases, runtime tests, and examples. Borrowed/shared/unique ownership policies, reset policy, and thread-local future-backend storage remain gaps. | API decisions, tests, and docs for borrowed/reference/shared/unique ownership, reset behavior, and future parallel/thread-local storage. |
| 5 | Branch output compatibility/routing validation | Accepted validation evidence exists; keep the claim scoped to compile-time compatibility validation until final candidate logs are attached. | Candidate branch includes branch-output validation implementation plus compatible/incompatible compile-pass/compile-fail tests. |
| 6 | Join consumption/compatibility validation | Accepted validation evidence exists; keep the claim scoped to join consumption/compatibility validation, not runtime execution. | Candidate branch includes join validation implementation plus misuse/mismatch compile-fail tests. |
| 7 | Sequential branch execution | **Done for the supported slice.** Homogeneous outputs, heterogeneous outputs through `std::variant`, selected-output type-list joins, move-only selected-branch input consumption, selected-output join stages, explicit sequential fan-in joins with failed-case diagnostics, void-output cases, borrowed move-only fan-in, observer events, stateful storage, compile-time join validation, first thread-pool fan-in backend scheduling, runtime tests, and examples are all present. | Keep docs/examples aligned with supported boundary; preemptive cancellation, dependency backend fan-in, and broad backend branch execution remain future work. |
| 8 | Runtime descriptor/export contract | **Typed schema-version contract landed.** The `"pb.core.graph.v1"` identifier is now enforced by `tests/compile_pass/schema_v1_contract.cpp` and golden output is regression-locked in `tests/compile_pass/schema_v1_extended_golden.cpp`. A frozen external interchange schema, CLI/file export, and versioned migration policy remain roadmap-only. | Stability promise (no v1 field removal without major bump), schema-migration docs, CLI/file export, and compatibility tests on an external schema boundary. |
| 9 | DOT/JSON graph export | Partial descriptor-record-backed DOT/JSON helpers exist for linear, supported branch, and explicit fan-in pipelines; stable descriptor/export compatibility remains roadmap-only. | Candidate export evidence plus clear distinction between helper output and stable schemas, golden fixtures, CLI/file export, and backend graph-execution claims. |
| 10 | Backend feature matrix | Documented in `docs/optional-backends-roadmap.md`; `sequential` and the current standard-library `thread_pool_backend` fan-in slice are supported, while oneTBB, Taskflow, stdexec, preemptive cancellation, and broad backend examples/benchmarks remain roadmap-only. | Keep the matrix current before any backend implementation/support claim. |
| 11 | Full release compiler matrix | GitHub evidence exists for code SHA `87299c14c813753d170911239e251064cbbfee6f`: GCC C++20/C++23, Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release passed `163/163`. MSVC C++23, C++26 gates, and Windows package-release remain unclaimed. | Keep compiler ID/version plus configure/build/test/package evidence attached to the final candidate SHA; rerun if later non-doc code changes land. |
| 12 | Release/package evidence on candidate SHA | Clean Ubuntu package-release GitHub evidence exists for code SHA `87299c14c813753d170911239e251064cbbfee6f`: configure/build/CTest `163/163` and TGZ package generation passed. The same SHA also passed local `package-release-clang-ninja` configure/build/CTest `163/163` plus package generation at `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`. | Rerun package-release on the final tag SHA when required and record archive path plus workflow URL. |

## Current support summary

The current repository can safely claim:

- linear typed pipeline validation
- explicit stage metadata and compile-time introspection helpers
- sequential runtime execution for validated linear pipelines
- sequential branch execution with optional join stages, heterogeneous outputs through `std::variant`, explicit sequential fan-in with skipped/completed/failed slots, borrowed move-only fan-in, move-only selected-branch input consumption, and the standard-library `thread_pool_backend` fan-in scheduling slice
- `pb::shared_view<T>` copy-cheap shared-ptr-backed wrapper for non-copyable fan-in inputs, with `pb::fan_in_uses_copy_v` / `pb::fan_in_uses_borrow_v` introspection
- `pb::projected<From, Projection, Stage>` projection adapter wrapping an existing stage with a user-supplied projection callable
- error-policy DSL: five factory-built engine wrappers — `pb::with_throw_on_error` → `throwing_engine`, `pb::with_terminate_on_error` → `terminating_engine`, `pb::with_ignore_errors` → `ignoring_engine`, `pb::with_propagate_exceptions` → `propagating_engine`, `pb::with_verbose_diagnostics` → `verbose_engine` (owns `pb::verbose_observer`); cross-wrapper `run()` / `try_run()` parity proven over linear, branch, and fan-in topologies; composability shipped (`verbose` wraps any of the other four because all five expose `set_observer` / `get_observer` / `describe` / `descriptor` directly); `std::expected`-shape stages flow through every wrapper with `error_category::expected_error` normalization
- `pb.core.graph.v1` typed schema-version contract enforced by compile-time regression tests; `pb::export_text` text-format export helper
- typed C++26 / C++23 feature gate constants (`pb::features::has_cpp26`, `has_reflection`, `has_contracts`, `has_pack_indexing`, `has_std_expected`, `has_deducing_this`) alongside their `PB_HAS_*` macro counterparts and implication invariants
- reflection adapter scaffold in `include/pb/adapt/reflect.hpp` gated on `PB_HAS_REFLECTION` with a graceful C++20 fallback
- substantially expanded `pb_cli` with `tests/run_pb_cli_describe.cmake` test driver
- compile-pass, compile-fail, runtime, example, package-consumer, and benchmark smoke scaffolding
- current working tree at **200/200 local ctest** (includes the error-policy DSL parity-hardening batch + the release-readiness pack: schema v1 stability promise + cross-version regression, observer ABI `pb.observer.v1` + verbose line-schema `pb.observer.verbose.v1`, `pb.error.v1` JSON serialization, `pb.trace.ndjson.v1` streaming `pb::tracing_sink`, expanded compile-fail diagnostic goldens, recorded compile-time benchmark baselines); cross-compiler validation on the new SHA pending
- release-readiness documentation plus GitHub GCC/Clang/MSVC/package validation evidence and local package evidence for code SHA `87299c14c813753d170911239e251064cbbfee6f`

The current repository should **not** claim:

- production-complete topology or execution coverage
- dependency-backend fan-in join execution beyond the thread-pool slice, a frozen external descriptor/export schema, fully stabilized observer contracts, or a stable runtime descriptor
- a runtime-enforced `::with<pb::policy::...>` DSL over every policy axis (the wrapper-layer error-policy DSL is shipped and parity-proven; the broader `::with<pb::policy::...>` runtime semantics beyond marker bundles remain roadmap-only)
- benchmark thresholds or CI-enforced performance budgets
- MSVC C++23, C++26 feature implementation, Windows package-release, or package-manager ecosystem validation
- Taskflow, oneTBB, or stdexec backend support
- C++26 reflection as supported — the adapter scaffold is gated and falls back on C++20 baselines
- preemptive cancellation or broad backend branch execution
- fully frozen diagnostics across all future slices
- a published release or tagged version

## How to use this map in the docs lane

When adding a new status page or release-facing note:

1. Start by finding the theme above.
2. Reuse only the proof points that already exist in the tree.
3. Mark any missing capability as roadmap-only unless code, tests, and examples all support the claim.
4. Prefer one narrow page or one narrow link update over broad cross-doc churn.

That keeps the documentation truthful while the repo continues moving from MVP scaffolding toward wider roadmap coverage.

## Next long-horizon queue

Use these as the first finite batches when the next 3-agent team resumes:

1. **Core API / diagnostics:** improve one small compile-time diagnostics or metadata ergonomics gap, then add compile-pass/compile-fail evidence.
2. **Runtime / adapters:** harden one small result/error/observer/adapter edge case, then add targeted runtime coverage.
3. **Updater / docs:** keep docs, examples, release notes, and this map aligned with the coding batches without promoting roadmap-only features.

The next safe work should continue from shipped MVP surfaces. Preemptive cancellation, dependency-backend fan-in / true backend multi-input join execution beyond the thread-pool slice, broader move-only ownership policies, backend branch execution, and stable descriptor/export compatibility remain separate design/implementation phases, not opportunistic follow-ups inside a routine hardening batch.
