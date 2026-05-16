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
| Branch/join topology | Marker-only `case_` / `branch_case` / `branch_node` / `join_node` aliases plus `branch_case_output` / `branch_outputs` marker metadata, branch source compatibility, predicate marker validation, homogeneous branch-node case-input validation, invalid join-stage diagnostics, and branch-output marker misuse diagnostics now lock the unsupported boundary from research lines 468-479, 583-591, 989-997, and 1451-1462. No branch output compatibility validation, join consumption validation, graph export, or sequential branch execution is shipped. | Compile-pass public-header marker checks and compile-fail unsupported-boundary, branch source/predicate, branch-node case-input mismatch, invalid join-stage, and branch-output marker misuse diagnostics; `docs/branch-join-roadmap.md`. | Roadmap-only for executable branch/join topology; marker metadata/diagnostics only. | Implement branch output compatibility/routing validation, join consumption validation, sequential branch execution, examples, and release-facing docs before support claims change. |
| Graph export | No graph export contract is currently shipped. | Roadmap-only docs: `docs/graph-export-roadmap.md`. | Roadmap-only. | Define stable descriptor/export schema after branch/join and descriptor semantics are settled. |
| Stable runtime descriptor | Current describe/metadata helpers support linear introspection, descriptor alias symmetry coverage, runtime `error_record` / `to_record(...)` diagnostic projection, and tests that align descriptor stage identity with observer failure/exception callbacks, custom expected-like diagnostic annotation, and the `try_run()` error envelope, but no stable runtime descriptor/export contract is promised. | `include/pb/core/describe.hpp`, `include/pb/runtime/error.hpp`, descriptor compile-pass tests, runtime diagnostic smoke tests, linear descriptor/observer identity smoke tests, `docs/runtime-descriptor-roadmap.md`. | Partial MVP introspection only; stable descriptor remains roadmap. | Define ownership, value schema, versioning, and export compatibility before presenting descriptor output as a stable public contract. |
| Optional backends | No thread-pool, oneTBB, Taskflow, or stdexec executor is supported today. | Roadmap-only docs: `docs/optional-backends-roadmap.md`. | Roadmap-only. | Build backend feature matrix first, then prototype one backend without leaking backend APIs into core headers. |
| Benchmark thresholds and compile-time budgets | Benchmark smoke targets and Clang time-trace workflow exist, but no release threshold or regression budget is enforced. | `bench/*`, `docs/benchmark-workflow.md`, `docs/benchmark-evidence-template.md`, CTest benchmark labels. | Smoke/profiling scaffolding only. | Record baseline numbers for 5-stage and 50-stage chains, then add CI/regression budget policy before treating performance as a release gate. |
| Release governance | CMake install/package config smoke scaffolding and release checklist exist. | `docs/release-readiness-checklist.md`, `docs/release-evidence-template.md`, package presets, `pb_package_config_smoke`. | Release scaffolding shipped; release readiness requires fresh candidate evidence. | Run package-release configure/build/CTest/package on final candidate SHA and record compiler/preset/archive evidence before tagging. |

## Current release decision rule

A release note may claim the linear/sequential MVP, current adapter shapes, smoke benchmark scaffolding, and package-consumer scaffolding only when fresh candidate evidence is attached. It must keep branch/join, graph export, stable runtime descriptor/export, optional backends, benchmark thresholds, stable observer ABI/event schema, and CI-enforced performance budgets marked as roadmap gaps.

## Evidence refresh checklist

Before changing any row from roadmap/partial to shipped support, require all three:

1. Code/API evidence in the owned surface.
2. Targeted compile-pass, compile-fail, runtime, benchmark, or package tests proving the behavior.
3. Documentation/examples that state the boundary without promoting future roadmap behavior.
