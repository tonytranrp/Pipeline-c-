# Roadmap Gap Map

This page maps the major themes in `research/pipeline_builder_cpp_research_plan.md` to the repository evidence that exists **today**. Use it to separate shipped MVP support from roadmap-only gaps, and to identify the next safe documentation or implementation slices without overstating production readiness.

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
- **Safe next slice:** keep runtime behavior, docs, and smoke coverage aligned while known `run()` / `try_run()` hardening work remains explicit.

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

- **Current repository evidence:** The docs explicitly mark non-linear topology as unimplemented roadmap work.
- **Current support level:** **Roadmap-only gap.**
- **Proof points:**
  - `docs/branch-join-roadmap.md`
  - known-gap references in `docs/production-readiness.md`
  - research-plan branch/join themes
- **Safe next slice:** keep follow-on work isolated to status/docs until public API, validation, execution, and tests exist.

### 6. Graph export

- **Current repository evidence:** Graph export is documented as roadmap-only and not a supported current surface.
- **Current support level:** **Roadmap-only gap.**
- **Proof points:**
  - `docs/graph-export-roadmap.md`
  - `docs/production-readiness.md`
  - research-plan export themes
- **Safe next slice:** only claim export support after descriptor/export APIs, tests, and examples land together.

### 7. Observer hooks

- **Current repository evidence:** Observer hooks are documented as future instrumentation/inspection work, not as a shipped API.
- **Current support level:** **Roadmap-only gap.**
- **Proof points:**
  - `docs/observer-hooks-roadmap.md`
  - `docs/production-readiness.md`
- **Safe next slice:** continue roadmap/status documentation until there is a concrete observer surface plus runtime validation.

### 8. Optional backends

- **Current repository evidence:** The repository explicitly limits current execution support to the standard-library sequential path and marks alternative backends as future work.
- **Current support level:** **Roadmap-only gap beyond sequential execution.**
- **Proof points:**
  - `docs/optional-backends-roadmap.md`
  - `docs/production-readiness.md`
  - `docs/build.md` notes that backend flags remain intentionally off in the base scaffold
- **Safe next slice:** keep dependency/runtime-backend claims off the supported surface until adapter targets, tests, and performance evidence exist.

### 9. Stable runtime descriptor

- **Current repository evidence:** Compile-time introspection exists today, but a stable runtime-facing descriptor/export contract does not.
- **Current support level:** **Roadmap-only gap.**
- **Proof points:**
  - `docs/runtime-descriptor-roadmap.md`
  - `docs/production-readiness.md`
  - `include/pb/core/describe.hpp`
- **Safe next slice:** keep compile-time metadata documentation separate from any future runtime descriptor claim until a stable public contract is defined and tested.

### 10. Benchmark evidence and performance budgets

- **Current repository evidence:** Benchmark lanes are documented as smoke/profiling scaffolding, not release thresholds.
- **Current support level:** **Shipped smoke scaffolding with roadmap-only performance governance.**
- **Proof points:**
  - `docs/build.md`
  - `docs/benchmark-workflow.md`
  - `docs/release-readiness-checklist.md`
- **Safe next slice:** collect reproducible benchmark evidence with context, but keep thresholds/regression budgets labeled as future work until explicitly established.

## Current support summary

The current repository can safely claim:

- linear typed pipeline validation
- explicit stage metadata and compile-time introspection helpers
- sequential runtime execution for validated linear pipelines
- compile-pass, compile-fail, runtime, example, package-consumer, and benchmark smoke scaffolding
- release-readiness documentation that explains what evidence must be collected before tagging

The current repository should **not** claim:

- production-complete topology or execution coverage
- branch/join, graph export, observer hooks, optional backend execution, or a stable runtime descriptor
- benchmark thresholds or CI-enforced performance budgets
- fully frozen diagnostics across all future slices

## How to use this map in the docs lane

When adding a new status page or release-facing note:

1. Start by finding the theme above.
2. Reuse only the proof points that already exist in the tree.
3. Mark any missing capability as roadmap-only unless code, tests, and examples all support the claim.
4. Prefer one narrow page or one narrow link update over broad cross-doc churn.

That keeps the documentation truthful while the repo continues moving from MVP scaffolding toward wider roadmap coverage.
