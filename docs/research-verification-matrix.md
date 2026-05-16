# Research Verification Matrix

This matrix maps the current repository evidence to the remaining roadmap gaps in `research/pipeline_builder_cpp_research_plan.md`. It is release-facing evidence control, not an implementation plan by itself.

## Scope and source anchors

Source-of-truth research anchors with line ranges from `research/pipeline_builder_cpp_research_plan.md`:

- Lines 989-997, Phase 5 branch/join requirements: type-level branch node, predicate requirements, branch output validation, join validation, sequential branch execution, and branch diagnostics.
- Lines 998-1005, Phase 6 optional backend requirements: thread-pool executor plus oneTBB, Taskflow, stdexec prototypes, and a backend feature matrix.
- Lines 1006-1013, Phase 7 packaging/release requirements: install targets, package config files, examples, docs, CI compiler matrix, and v0.1.0 publication.
- Lines 1324-1336, acceptance criteria: sequential valid chains, compile-time invalid edge diagnostics, expected-like failure propagation, adapters, throwing-function runtime errors, GCC/Clang public-header coverage, compile-time benchmark records, and one success plus one failure example.
- Lines 1490-1501 and 1518, compile-time optimization notes: compile-time baselines/regression tracking and release-quality benchmark gates remain explicit goals.

## Gap-to-evidence matrix

| Research area | Current implemented evidence | Current test/evidence surface | Release status | Next required slice |
| --- | --- | --- | --- | --- |
| Linear MVP core | Typed linear `pb::from<T>::then<S>::to<U>` DSL, stage traits, describe/metadata helpers, validation diagnostics, and public aliases. | Compile-pass/compile-fail suites, public-header smoke tests, examples, and `docs/roadmap-gap-map.md`. | Shipped MVP support. | Keep diagnostics wording hardened and refresh compile-fail evidence on each release candidate. |
| User adapters | Free-function, function-object, functor/member adapter surfaces with recent adapter/member hardening in `include/pb/adapt/fn.hpp`. | Adapter compile-pass/compile-fail tests and runtime adapter smoke tests. | Shipped MVP support for current adapter shapes. | Keep adapter docs limited to supported callable shapes; add examples only when they match tested behavior. |
| Runtime result/error model | Sequential runtime, `run()` / `try_run()`, expected-like/result normalization, custom error conversion, observer callbacks, and exception capture boundaries. | Runtime smoke tests, result tests, observer smoke tests, `docs/error-model-tutorial.md`, and `docs/observer-hooks-roadmap.md`. | Shipped sequential MVP support with policy gaps called out. | Harden `run()`/`try_run()` policy wording and avoid claiming stable observer ABI/event schema. |
| Branch/join topology | Marker-only `case_` / `branch_case` / `branch_node` / `join_node` aliases plus `branch_case_output` / `branch_outputs` marker metadata, branch source compatibility, predicate marker validation, homogeneous branch-node case-input validation, branch-output compatibility validation, join consumption validation, invalid join-stage diagnostics, and branch-output marker misuse diagnostics now lock the unsupported boundary from research lines 468-479, 583-591, 989-997, and 1451-1462. Sequential branch execution is promoted for planning but is not shipped. | Compile-pass public-header marker checks and compile-fail unsupported-boundary, branch source/predicate, branch-node case-input mismatch, branch-output mismatch, invalid join-stage, join-validation mismatch/misuse, and branch-output marker misuse diagnostics; `docs/branch-join-roadmap.md`. | Validation support only; executable branch/join topology remains roadmap-only. | Keep branch output/join validation evidence attached to the candidate branch, then implement sequential branch execution, examples, and release-facing docs before execution support claims change. |
| Graph export | Leader-accepted evidence covers a narrow linear DOT export/helper slice only. Full graph export, JSON export, branch/join graph export, stable graph schema, and stable runtime descriptor/export contracts remain unshipped. | Targeted DOT evidence from the export lane plus roadmap-only docs: `docs/graph-export-roadmap.md`. | Linear DOT only when final candidate verification includes the accepted slice; full graph export remains roadmap-only. | Define stable descriptor/export schema after branch/join and descriptor semantics are settled, then add JSON/graph-shaped tests and examples before broader claims. || Stable runtime descriptor | Current describe/metadata helpers support linear introspection, descriptor alias symmetry coverage, runtime `error_record` / `to_record(...)` diagnostic projection, and tests that align descriptor stage identity with observer failure/exception callbacks, custom expected-like diagnostic annotation, and the `try_run()` error envelope, but no stable runtime descriptor/export contract is promised. | `include/pb/core/describe.hpp`, `include/pb/runtime/error.hpp`, descriptor compile-pass tests, runtime diagnostic smoke tests, linear descriptor/observer identity smoke tests, `docs/runtime-descriptor-roadmap.md`. | Partial MVP introspection only; stable descriptor remains roadmap. | Define ownership, value schema, versioning, and export compatibility before presenting descriptor output as a stable public contract. |
| Optional backends | No thread-pool, oneTBB, Taskflow, or stdexec executor is supported today. | Roadmap-only docs: `docs/optional-backends-roadmap.md`. | Roadmap-only. | Build backend feature matrix first, then prototype one backend without leaking backend APIs into core headers. |
| Benchmark thresholds and compile-time budgets | Benchmark smoke targets and Clang time-trace workflow exist, but no release threshold or regression budget is enforced. | `bench/*`, `docs/benchmark-workflow.md`, `docs/benchmark-evidence-template.md`, CTest benchmark labels. | Smoke/profiling scaffolding only. | Record baseline numbers for 5-stage and 50-stage chains, then add CI/regression budget policy before treating performance as a release gate. |
| Release governance | CMake install/package config smoke scaffolding and release checklist exist. | `docs/release-readiness-checklist.md`, `docs/release-evidence-template.md`, package presets, `pb_package_config_smoke`. | Release scaffolding shipped; release readiness requires fresh candidate evidence. | Run package-release configure/build/CTest/package on final candidate SHA and record compiler/preset/archive evidence before tagging. |

## Active priority backlog for release coordination

The docs lane tracks these priorities so release notes and PR text do not overclaim in-progress work from other workers:

| Priority | Research-plan anchor | Status to claim today | Required evidence before promotion |
| --- | --- | --- | --- |
| 1-2 | Phase 2 runtime result/error and expected-like behavior | Sequential MVP support with explicit policy gaps. | Runtime tests and docs that prove the final `run()` / `try_run()` / `std::expected` behavior on supported compilers. |
| 3 | Compile-time optimization notes, lines 1490-1501 and 1518 | Benchmark smoke scaffolding only. | Baseline records for 5-stage and 50-stage chains plus context. |
| 4 | Runtime stage ownership/lifetime assumptions | No stable stateful-storage policy guarantee. | API policy, tests, and docs. |
| 5-6 | Phase 5 branch output and join validation | Accepted validation evidence exists for branch output compatibility and join consumption; keep it scoped to validation, not execution. | Final candidate branch includes the validation commits plus branch-output and join-validation compile-pass/compile-fail logs. |
| 7 | Phase 5 sequential branch execution | Promoted for tests-first planning after validation-gate worker reports; still not shipped support. | Minimal linear lowering plan, runtime execution tests, examples, and release-doc updates after integration. |
| 8-9 | Descriptor/export and graph export themes | Compile-time metadata exists and a narrow linear DOT slice is accepted; full graph export remains roadmap-only. | Stable schema/export API and tests; distinguish linear DOT helpers from full graph export. |
| 10 | Phase 6 optional backends | Sequential backend only; backend feature matrix is documented. | Implementation, isolation, tests, examples, and benchmark evidence before any new backend support claim. |
| 11-12 | Phase 7 compiler/release requirements | Release compiler matrix is documented; release/package evidence still must be collected on the candidate SHA. | Fresh compiler ID/version plus configure/build/test/package logs before tagging. |## Current release decision rule

A release note may claim the linear/sequential MVP, current adapter shapes, smoke benchmark scaffolding, package-consumer scaffolding, accepted branch-output/join validation, and a narrow linear-DOT helper only when fresh candidate evidence is attached. It must keep sequential branch execution, full branch/join graph export, JSON export, stable runtime descriptor/export, optional backends, benchmark thresholds, stable observer ABI/event schema, and CI-enforced performance budgets marked as roadmap gaps.

## Evidence refresh checklist

Before changing any row from roadmap/partial to shipped support, require all three:

1. Code/API evidence in the owned surface.
2. Targeted compile-pass, compile-fail, runtime, benchmark, or package tests proving the behavior.
3. Documentation/examples that state the boundary without promoting future roadmap behavior.
