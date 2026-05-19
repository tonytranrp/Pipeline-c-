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

- **Current repository evidence:** Public branch/join DSL with compile-time validation, runtime sequential branch execution (first-match-wins with short-circuit), observer events (`on_case_selected`, `on_case_skipped`, `on_case_failed`), stateful storage, join stages, error propagation, heterogeneous outputs through `std::variant`, selected-output type-list joins, explicit sequential fan-in with skipped/completed/failed slots, void-output cases, borrowed move-only fan-in, move-only selected-branch input consumption, and comprehensive tests and examples.
- **Current support level:** **Supported for homogeneous outputs, heterogeneous outputs through `std::variant`, selected-output type-list joins, explicit sequential fan-in, borrowed move-only fan-in, and move-only selected-branch input consumption.**
- **Proof points:**
  - `docs/branch-join-roadmap.md`
  - `include/pb/core/pipeline_state.hpp` (public `::branch<...>::join<...>` DSL)
  - `include/pb/runtime/sequential.hpp` (runtime branch routing with storage)
  - `tests/runtime/sequential_branch_comprehensive.cpp`
  - `examples/branch_routing_demo.cpp`, `examples/branch_error_handling.cpp`
- **Safe next slice:** keep backend/parallel fan-in, cancellation/scheduling policy, broader move-only predicate patterns, and backend branch execution as separate design/implementation phases.

### 6. Graph export

- **Current repository evidence:** DOT/JSON helper slices exist for linear and supported branch pipelines, including branch topology detection in JSON.
- **Current support level:** **Partial descriptor-record-backed export helper support; stable descriptor/export compatibility remains roadmap-only.**
- **Proof points:**
  - `docs/graph-export-roadmap.md`
  - `docs/production-readiness.md`
  - research-plan export themes
- **Safe next slice:** only claim broad export support after stable descriptor/export APIs, release-grade golden compatibility tests, schema docs, CLI/file behavior, and examples land together.
- **Coordination note:** the DOT/JSON helper APIs are narrower than full graph export. Release notes must distinguish them from stable graph schema support and backend graph-execution claims.

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

- **Current repository evidence:** A narrow linear runtime-descriptor helper exists today (`pb::make_descriptor<Pipeline>()` and `engine.descriptor()`), alongside compile-time introspection and runtime diagnostic stage-identity checks, but a stable runtime-facing descriptor/export contract does not.
- **Current support level:** **Partially shipped linear metadata support; stable export/compatibility contract remains roadmap-only.**
- **Proof points:**
  - `docs/runtime-descriptor-roadmap.md`
  - `docs/production-readiness.md`
  - `include/pb/core/describe.hpp`
  - `include/pb/runtime/descriptor.hpp`
  - `tests/compile_pass/public_headers/runtime_descriptor.cpp`
  - `tests/runtime/descriptor_smoke.cpp`
- **Safe next slice:** keep the current linear helper documented and tested at its real boundary while leaving broader export/versioning guarantees roadmap-only until a stable public contract is defined.

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
| 1 | `run()` / `try_run()` error-policy hardening and exception behavior evidence | Runtime lane in progress; current docs still describe split behavior. | Targeted runtime tests plus updated error-model docs proving the chosen policy. |
| 2 | `std::expected` / expected-like result policy evidence | Expected-like support exists, but direct standard-library matrix coverage remains a release-hardening gap. | Compile/runtime tests on the supported compiler/library matrix, or explicit fallback wording when `std::expected` is unavailable. |
| 3 | Compile-time benchmark baselines for 5-stage and 50-stage chains | Benchmark smoke scaffolding exists; baseline evidence and thresholds remain separate. | Recorded baseline artifacts with preset/compiler/commit context. |
| 4 | Explicit stateful stage storage policy | Narrow public guarantee exists for default-initializable sequential stages: per-run construction versus engine-stored state, including non-copyable owned stage state, is covered by policy aliases, runtime tests, and examples. Borrowed/shared/unique ownership policies, reset policy, and thread-local future-backend storage remain gaps. | API decisions, tests, and docs for borrowed/reference/shared/unique ownership, reset behavior, and future parallel/thread-local storage. |
| 5 | Branch output compatibility/routing validation | Accepted validation evidence exists; keep the claim scoped to compile-time compatibility validation until final candidate logs are attached. | Candidate branch includes branch-output validation implementation plus compatible/incompatible compile-pass/compile-fail tests. |
| 6 | Join consumption/compatibility validation | Accepted validation evidence exists; keep the claim scoped to join consumption/compatibility validation, not runtime execution. | Candidate branch includes join validation implementation plus misuse/mismatch compile-fail tests. |
| 7 | Sequential branch execution | **Done for the supported slice.** Homogeneous outputs, heterogeneous outputs through `std::variant`, selected-output type-list joins, move-only selected-branch input consumption, selected-output join stages, explicit sequential fan-in joins with failed-case diagnostics, void-output cases, borrowed move-only fan-in, observer events, stateful storage, compile-time join validation, runtime tests, and examples are all present. | Keep docs/examples aligned with supported boundary; backend/parallel fan-in, cancellation/scheduling policy, and backend branch execution remain future work. |
| 8 | Runtime descriptor/export contract | Compile-time/diagnostic metadata exists; stable runtime export remains roadmap. | Versioned schema, ownership rules, and tests for the public descriptor/export surface. |
| 9 | DOT/JSON graph export | Partial descriptor-record-backed DOT/JSON helpers exist for linear and supported branch pipelines; stable descriptor/export compatibility remains roadmap-only. | Candidate export evidence plus clear distinction between helper output and stable schemas, golden fixtures, CLI/file export, and backend graph-execution claims. |
| 10 | Backend feature matrix | Documented in `docs/optional-backends-roadmap.md`; backend support remains roadmap-only beyond sequential. | Keep the matrix current before any backend implementation/support claim. |
| 11 | Full release compiler matrix | GitHub evidence exists for code SHA `f56fa54`: GCC C++20/C++23, Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release passed. Current local HEAD `4d7127c` passed developer and package-release verification locally. MSVC C++23, C++26 gates, and Windows package-release remain unclaimed. | Keep compiler ID/version plus configure/build/test/package evidence attached to the final candidate SHA; rerun if later non-doc code changes land. |
| 12 | Release/package evidence on candidate SHA | Clean Ubuntu package-release GitHub evidence exists for code SHA `f56fa54`: configure/build/CTest `153/153` and TGZ package generation passed. Current local HEAD `4d7127c` also passed local `package-release-clang-ninja` configure/build/CTest `156/156` plus TGZ package generation. | Rerun package-release on the final tag SHA when required and record archive path plus workflow URL. |

## Current support summary

The current repository can safely claim:

- linear typed pipeline validation
- explicit stage metadata and compile-time introspection helpers
- sequential runtime execution for validated linear pipelines
- sequential branch execution with optional join stages, heterogeneous outputs through `std::variant`, explicit sequential fan-in with skipped/completed/failed slots, borrowed move-only fan-in, and move-only selected-branch input consumption
- compile-pass, compile-fail, runtime, example, package-consumer, and benchmark smoke scaffolding
- release-readiness documentation plus GitHub GCC/Clang/MSVC/package validation evidence for code SHA `f56fa54` and local current-HEAD package evidence for `4d7127c`

The current repository should **not** claim:

- production-complete topology or execution coverage
- parallel fan-in join execution, stable descriptor/export compatibility, fully stabilized observer contracts, optional backend execution, or a stable runtime descriptor
- exception-policy parity between `run()` and `try_run()` as a fully harmonized runtime guarantee
- benchmark thresholds or CI-enforced performance budgets
- MSVC C++23, C++26 feature implementation, Windows package-release, or package-manager ecosystem validation
- fully frozen diagnostics across all future slices

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

The next safe work should continue from shipped MVP surfaces. Parallel all-branches fan-in / true backend multi-input join execution, broader move-only predicate/ownership policies, backend branch execution, and stable descriptor/export compatibility remain separate design/implementation phases, not opportunistic follow-ups inside a routine hardening batch.
