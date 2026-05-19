# Current implementation map against `pipeline_builder_cpp_research_plan.md`

This file tracks what is done, partially done, still missing, and what must be finished before Pipeline-c++ can claim the full original research-plan vision.

Updated after the sequential fan-in hardening slice that adds failed-case aggregation, void-output case aggregation, borrowed move-only fan-in support, and fan-in case-failure observer/trace coverage. The latest exact-SHA cross-compiler evidence in this file still refers to `2751238f80ba102e559b395cef3e83406b21cb17`; rerun and refresh the validation block after this non-doc code slice is committed.

## Latest evidence snapshot

Current local evidence for validated code SHA `2751238f80ba102e559b395cef3e83406b21cb17`:

```text
- git diff --check: passed
- cmake --preset clang-dev-ninja: passed
- cmake --build --preset clang-dev-ninja --target pb_branch_fan_in_compile_pass pb_runtime_sequential_branch_fan_in: passed
- ctest --preset clang-dev-ninja --output-on-failure -R 'fan_in|fan-in|branch_fan|sequential_branch_fan': passed, 6/6
- cmake --build --preset clang-dev-ninja: passed
- ctest --preset clang-dev-ninja --output-on-failure: passed, 162/162
- cmake --fresh --preset dev-ninja: passed with GCC 13.3.0
- cmake --build --preset dev-ninja: passed
- ctest --preset dev-ninja --output-on-failure: passed, 162/162
- cmake --preset package-release-clang-ninja: passed
- cmake --build --preset package-release-clang-ninja: passed
- ctest --preset package-release-clang-ninja --output-on-failure: passed, 162/162
- cmake --build --preset package-release-clang-ninja --target package: passed
- package artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Latest GitHub validation for the same code SHA is current:

```text
- workflow: Cross Compiler Validation
- run: https://github.com/tonytranrp/Pipeline-c-/actions/runs/26130353198
- validated SHA: 2751238f80ba102e559b395cef3e83406b21cb17
- GCC C++20/C++23: passed, 162/162
- Clang C++20/C++23: passed, 162/162
- MSVC C++20: passed, 161/161
- clean Ubuntu package-release-clang-ninja: passed, 162/162 plus TGZ package generation
- package artifact path in runner: /home/runner/work/Pipeline-c-/Pipeline-c-/build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Normal CI also passed on the same SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26130353599>. The cross-compiler and CI logs had no compiler-style `warning:` diagnostics after the Release/NDEBUG smoke-test cleanup; GitHub still emits a hosted-runner Node 20 action annotation unrelated to compiler warnings.

## Done / production-grade for the current supported scope

These areas are implemented, tested, documented, and safe to present as supported when the release evidence is fresh.

| Area | Current status | Evidence / boundary |
| --- | --- | --- |
| CMake/package skeleton | Done for the current package shape. | `pb::core`, `pb::runtime`, and `pb::pipeline` install/export targets exist; package smoke validates downstream consumers and TGZ contents. |
| C++20 baseline | Done. | Target-based CMake requires C++20 and disables extensions. C++23 is validated in GitHub lanes, but C++20 remains the baseline. |
| Linear typed DSL | Done. | `pb::from<T>::then<S>::to<U>` and `::to<U>` are implemented with compile-pass and compile-fail coverage. |
| Stage traits and metadata | Done for current traits. | `input_type`, `output_type`, optional `error_type`, names, keys, public aliases, and descriptor helpers are covered. |
| Linear compile-time validation | Done for current linear graph. | Invalid stages, adjacent edge mismatch, sink mismatch, and public-header coverage are tested. |
| Sequential runtime | Done for validated linear pipelines. | `pb::compile<Pipeline>(pb::runtime::sequential{})`, `run()`, `try_run()`, move-only linear paths, and zero-stage identity behavior are tested. |
| Result / expected-like plumbing | Done for current supported behavior. | `pb::runtime::result`, expected-like normalization, stage failure propagation, custom error conversion, and exception capture boundaries are tested. Broader policy DSL remains separate. |
| Observer hooks for sequential runtime | Done for current sequential hooks. | Stage start/success/failure/exception and branch case events are covered. Stable ABI/event-schema guarantees remain future work. |
| User-code adapters | Done for current adapter shapes. | Free-function, member-function, and function-object/functor adapters are covered by compile-pass, compile-fail, runtime, docs, and examples. |
| Branch / selected-output join surface | Done for the supported sequential slice. | Public `::branch<...>` and `::join<...>`, homogeneous outputs, `std::variant` heterogeneous outputs, duplicate variant alternatives by index, selected-output type-list joins, observer case events, stateful storage, move-only selected-branch input consumption, examples, and tests are present. |
| Sequential fan-in join surface | Done for the current sequential surface. | Public `::fan_in<JoinStage>` and `::join_all<JoinStage>` follow `::branch<...>`, evaluate all predicates, run every passing case in declaration order, continue after predicate/stage failures, and pass `pb::fan_in_results<pb::fan_in_case_result<I, T>...>` to the join stage. Targeted compile-pass, runtime, and compile-fail tests cover zero/one/many passing cases, duplicate same-type case outputs by index, stateful storage, invalid fan-in joins, failed-case diagnostic aggregation, void-output cases, copy-requiring non-copyable fan-in rejection, and borrowed move-only fan-in. |
| Branch case identity metadata | Done for helper/export preparation. | Descriptor branch case records carry deterministic case and node identities plus labels. Runtime fallback identities exist for unnamed branch children. |
| DOT/JSON helper export | Done as a helper surface for supported shapes. | Linear and selected-output branch helper output uses `schema_version = "pb.core.graph.v1"`, branch topology, branch cases, JSON escaping, DOT escaping, and golden tests. This is not yet a stable external interchange contract. |
| Compile-time benchmark smoke scaffolding | Done as smoke/build evidence. | Header inclusion, 5-stage chain, 50-stage chain, aggregate `pb_compile_time_benchmarks`, and CTest labels exist. Timing budgets are not established. |
| Release/package verification scaffolding | Done for the current candidate evidence. | Package release preset, package smoke, GitHub cross-compiler workflow, release docs, evidence templates, exact code-SHA GitHub matrix evidence, and warning-clean Release smoke checks exist. Publication is still not done. |

## Implemented but still partial / almost production-grade

These areas work in meaningful slices, but should not be described as fully production-grade until the missing items in the right column are finished.

| Area | Implemented now | Missing before maximum / production grade |
| --- | --- | --- |
| Branch/join execution | First-match selected-output sequential branch execution, optional join stages, homogeneous outputs, variant heterogeneous outputs, type-list selected-output joins, explicit sequential fan-in joins, failed-case fan-in aggregation, void-output fan-in aggregation, borrowed move-only fan-in, stateful branch storage, observer events, and move-only selected-stage consumption. | Backend/parallel branch execution; backend ordering/cancellation rules; richer backend fan-in policy knobs; full backend branch tests. |
| Heterogeneous branch outputs | `std::variant` preserves duplicate alternatives by index; type-list selected-output joins dispatch by C++ type and intentionally share overloads for duplicate same-type outputs unless the user encodes case identity into the output type. Sequential fan-in distinguishes duplicate same-type case outputs by case index in `fan_in_case_result<I, T>` and can carry failed diagnostics per case. | Tagged case-result model for selected-output joins if needed; backend/fan-in error-envelope policies; docs/examples for all future policies. |
| Move-only branch input | Predicates observe by `const input_type&`; selected branch stage may consume move-only input. Compile-fail coverage rejects predicates that consume move-only inputs by value. Fan-in supports move-only inputs only when every passing case stage borrows by `const input_type&`; by-value fan-in still requires a copy-constructible input and has targeted compile-fail coverage. | Broader clone/shared-view/projection policies; resource lifetime docs; more observer/error edge cases; fan-in policies for stages that need owned per-case copies of non-copyable inputs. |
| Runtime descriptor/export | Runtime descriptor records and helper export fields are useful for current linear/branch helper output. | Stable versioned descriptor schema, compatibility rules, ownership/lifetime guarantees, unsupported-shape diagnostics, and release-grade descriptor examples. |
| DOT/JSON export | Helper output is golden-tested and documented for supported linear and selected-output branch shapes. | Long-term schema compatibility promise, CLI/file export contract, backend/fan-in graph export, schema migration rules, and published compatibility fixtures. |
| Runtime error model | `try_run()` captures supported failures into result-like output; expected-like propagation and runtime error formatting exist. | Full `::with<pb::policy::...>` error policy DSL, no-exception policy, `throw_on_error`/`terminate_on_error`, serialized error records, complete std::expected matrix wording, and fully harmonized `run()` / `try_run()` policy docs. |
| Diagnostics | Compile-fail tests cover important current structural errors; runtime errors carry stage identity. | Stable diagnostic wording/versioning, machine-readable diagnostic schema, suggested-fix catalog, cross-compiler diagnostic parity, branch/fan-in/backend diagnostic matrix. |
| Observer/tracing | Sequential observer hooks and trace-related smoke coverage exist. | Stable observer ABI/event schema, observer ownership/lifetime contract, tracing sink API, trace file format, cross-executor behavior, and observer/trace overhead benchmarks. |
| Stateful stage storage | Sequential stateful storage preserves linear stages and branch predicates/stages under the current policies. | Borrowed/shared/unique ownership policies, reset policy, thread-local future-backend storage, external-resource cleanup policy, and public lifetime diagnostics. |
| Adapters | Free-function, member-function, function-object/functor, expected-like/result paths, and current void/error behavior are covered. | Coroutine adapter, sender/receiver adapter, C API ownership/error adapter, runtime-bound callable adapter, full overloaded/ref-qualified member handling, reference-lifetime adapter, and policy docs. |
| Compile-time performance | Smoke targets prove representative translation units build; CMake has time-trace support. | Recorded release timing baselines, branch/join compile-time benchmark, thresholds, CI regression budget, ftime-trace aggregation, compile-time dashboard, and IWYU enforcement. |
| Cross-compiler validation | GitHub workflow covers GCC/Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release for validated code SHA `2751238`. | MSVC C++23 if supported; Windows package-release; experimental C++26 feature-gate evidence if those gates are claimed; rerun if later non-doc code changes land. |

## Not implemented yet / roadmap-only

These items remain outside the supported product surface.

### 1. Fan-in / all-branches joins beyond the first sequential slice

The repository now has an explicit sequential fan-in slice: `::fan_in<JoinStage>` and `::join_all<JoinStage>` evaluate every predicate, run every passing case in declaration order, allow zero passing cases, aggregate skipped/completed/failed slots with diagnostics, support void-output cases, and invoke the join with an ordered `pb::fan_in_results<...>` aggregate. This is a real implemented feature, but it is not the full research-plan graph/backend fan-in story yet.

Still need to implement:

```text
- backend/parallel failure aggregation and cancellation policy once parallel/backend fan-in exists
- clone/projection/shared-view policy for fan-in stages that need owned non-copyable inputs
- observer events beyond the current selected/skipped/failed callbacks, such as case scheduled/completed and join started/completed
- descriptor/export topology for fan-in graphs
- backend fan-in scheduling after sequential semantics stabilize
- backend compile-pass, compile-fail, runtime, docs, and benchmark coverage
```

### 2. Optional pipeline execution backends

Only the standard-library sequential pipeline executor is supported. `pb::runtime::thread_pool` is a standalone utility and readiness building block, not a pipeline backend.

Need to implement:

```text
- thread-pool pipeline backend
- backend lowering from validated graph to scheduled tasks
- backend state storage rules
- backend error/cancellation policies
- backend observer behavior
- backend examples and benchmarks
- oneTBB backend prototype
- Taskflow backend prototype
- stdexec / sender-receiver experimental backend
- dependency isolation target tests
```

### 3. Full policy DSL

Policy seams exist, but the research-plan `::with<pb::policy::...>` model is not complete.

Need to implement:

```text
- ::with<pb::policy::...> chaining on pipeline state
- error policy DSL
- exception policy DSL
- copy/move/clone policy
- reference lifetime policy
- state storage policy
- diagnostic verbosity policy
- executor capability policy
- assertion/contract policy
- policy conflict diagnostics
```

### 4. Stable external graph/export contract

Current DOT/JSON helpers are documented and tested, but still helper output rather than a stable interchange format.

Need to implement:

```text
- versioned public JSON schema
- versioned public DOT schema or documented DOT contract
- compatibility tests that fail on unintended schema changes
- schema migration rules
- stable descriptor-backed export API
- CLI/file export behavior
- examples for linear, selected-output branch, and future fan-in graphs
- unsupported-shape diagnostics
```

### 5. Graph visualization CLI

Current helper APIs exist, but the research-plan CLI remains future work.

Need to implement:

```text
- documented CLI command surface
- export to stdout and file
- DOT and JSON format options
- filtered graph view if desired
- CLI tests
- CLI docs/examples
```

### 6. C++ modules

Need to implement later:

```text
- module interface units
- module build/install/export story
- module compatibility tests
- documentation comparing headers vs modules
```

### 7. C++26 experimental features

The C++20 path is canonical. C++26 features remain opt-in future gates.

Need to implement later:

```text
- reflection adapter feature gate
- contracts integration feature gate
- pack-indexing optimization gate
- richer static_assert-message gate
- compiler/library detection tests
- fallback behavior for unsupported compilers
```

### 8. Release publication

Release machinery and local/package evidence exist, but a public release is not published.

Need to finish:

```text
- decide final release candidate SHA
- rerun cross-compiler validation on that SHA if needed
- update release notes with exact supported/unsupported boundaries
- tag v0.1.0
- publish GitHub release
- attach package/evidence as appropriate
```

## Production-grade priority order

Best next steps from lowest risk / highest production-readiness value to highest complexity:

```text
1. Preserve current exact-SHA GitHub validation evidence in release notes and rerun it only if later non-doc code changes land.
2. Record reproducible compile-time timing baselines for header, 5-stage, and 50-stage smoke targets.
3. Convert DOT/JSON helper schema from “documented helper” toward “stable supported schema” only after compatibility policy is explicit.
4. Harden runtime error policy docs/tests around run(), try_run(), expected-like, and exception boundaries.
5. Expand diagnostic golden coverage and suggested-fix docs for supported errors.
6. Add graph export CLI only after stable helper/schema rules are decided.
7. Finalize state/lifetime policies for owned, borrowed, shared, reset, and resource-owning stages.
8. Extend fan-in into descriptor/export topology docs and define clone/projection/shared-view policies for owned non-copyable inputs.
9. Prototype thread-pool pipeline backend after fan-in and policy boundaries are clear.
10. Add optional oneTBB/Taskflow/stdexec backends only after the thread-pool backend proves the lowering model.
11. Add modules and C++26 gates after baseline release and compiler matrix evidence.
12. Publish v0.1.0 when release notes, exact-SHA validation, package artifact, and unsupported-boundary docs all agree.
```

## Current scorecard

```text
Linear MVP foundation:                    9.0 / 10
Sequential runtime MVP:                   8.0 / 10
Result/error current surface:             7.3 / 10
Adapters current surface:                 7.5 / 10
Diagnostics current surface:              7.6 / 10
Selected-output branch/join execution:    8.6 / 10
Sequential fan-in join execution:          8.2 / 10
Move-only selected-branch input support:  7.6 / 10
Stateful branch storage:                  8.4 / 10
DOT/JSON helper export:                   8.0 / 10
Stable descriptor/export contract:        3.8 / 10
Compile-time benchmark scaffolding:       7.2 / 10
Optional pipeline backends:               0.8 / 10
Release readiness before tag:             8.4 / 10
Full original research-plan completion:   6.1 / 10
```

## Main conclusion

Pipeline-c++ has moved beyond a basic linear MVP: it now has validated linear pipelines, a sequential runtime, adapters, result/error plumbing, observer hooks, selected-output branch/join execution, descriptor-backed DOT/JSON helper output, compile-time smoke targets, package smoke, and cross-compiler workflow scaffolding.

It is still not the full research-plan product. The remaining maximum-production work is concentrated in promoting sequential fan-in into backend/parallel fan-in with cancellation/scheduling policies, descriptor/export contracts, optional pipeline backends, full policy/lifetime/error semantics, release-grade performance budgets, C++ modules/C++26 gates, and final exact-SHA release publication.
