# Current PR / Release Summary

Status snapshot for the current branch/export, compile-time benchmark, sequential fan-in hardening, Release/NDEBUG smoke coverage, and validation batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Current branch: `main`.
- Current validated code SHA: `8ae7d596facc59873dc75a31d5c23cdc8cf06763` (`Harden fan-in so joins can inspect every case`).
- Parent fan-in feature SHA: `cfaa7e96b50e54aa83e26c5fa70a858caf270880` (`Add explicit fan-in to close the branch graph gap`).
- GitHub cross-compiler validation: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26132278315> — **passed**.
- Normal CI workflow: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26132264545> — **passed**.
- Local verification paired with this code: Clang developer full CTest `162/162`, GCC/default developer full CTest `162/162`, package-release full CTest `162/162`, and package artifact `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`.
- Compiler-warning audit: cross-compiler and CI logs for this candidate had no compiler-style `warning:` diagnostics; GitHub still emits a hosted-runner Node 20 action annotation unrelated to project compiler warnings.

## What can be claimed with current evidence

- Linear typed pipeline validation and sequential runtime execution remain supported.
- Public branch/join DSL is supported for the selected-output sequential runtime slice.
- Homogeneous branch outputs and variant-based heterogeneous branch outputs are supported, including duplicate output alternatives preserved by `std::variant` index.
- Type-list selected-output joins dispatch by C++ type; duplicate same-type branch outputs share the same overload unless the user encodes case identity into the output type.
- Move-only selected-branch input consumption is supported when predicates inspect by `const input_type&`; consuming predicates for move-only inputs remain unsupported and have negative compile-fail coverage.
- Explicit sequential fan-in joins are supported through `::fan_in<JoinStage>` / `::join_all<JoinStage>`; all predicates are evaluated, all passing cases run in declaration order, zero passing cases are valid, duplicate same-type outputs are distinguished by case index in `pb::fan_in_case_result<I, T>`, failed predicate/stage cases carry diagnostics, void-output cases aggregate without placeholder values, borrowed move-only fan-in is supported when every case stage accepts `const input_type&`, and backend/parallel fan-in remains roadmap-only.
- Stateful branch predicates/stages are covered under `pb::runtime::stateful_sequential`, and predicate invocation uses const-input semantics in both per-run and stateful paths.
- DOT/JSON helpers cover linear and selected-output branch pipelines, including descriptor-record-backed branch helper rendering, JSON branch topology detection, current top-level JSON fields (`schema_version`, `topology`, `stage_count`, `edge_count`, `stages`, `edges`), branch case identity fields, DOT label escaping, JSON string escaping, and helper-output golden regressions for `pb.core.graph.v1`.
- Compile-time/header smoke targets cover public header inclusion plus representative 5-stage and 50-stage chains through the `pb_compile_time_benchmarks` aggregate target.
- Release/NDEBUG smoke tests keep their checks active with explicit aborting test helpers instead of assert-only variables.
- `pb::runtime::thread_pool` is a standalone utility only; it is not a pipeline backend.

## What must stay roadmap-only

- Backend/parallel fan-in / true backend multi-input join execution beyond the current sequential fan-in join model, plus backend cancellation/scheduling/error policy extensions.
- Stable descriptor/export schemas and release-grade compatibility fixtures beyond the current descriptor-record-backed helper output.
- CLI/file export as a stable public contract for user pipeline definitions.
- Thread-pool, oneTBB, Taskflow, or stdexec pipeline executor backends.
- Full policy DSL for error, exception, state, reference lifetime, diagnostic verbosity, and executor capability behavior.
- C++ modules and C++26 reflection/contracts feature integrations.
- Stable/frozen diagnostic wording across all future features.
- Benchmark thresholds, dashboards, release timing baselines, or CI-enforced compile-time/runtime performance budgets.

## Validation evidence collected

Current local verification for validated code SHA `8ae7d596facc59873dc75a31d5c23cdc8cf06763`:

```text
git diff --check: passed
cmake --build --preset clang-dev-ninja: passed
ctest --preset clang-dev-ninja --output-on-failure: passed, 162/162
cmake --build --preset dev-ninja: passed
ctest --preset dev-ninja --output-on-failure: passed, 162/162
cmake --build --preset package-release-clang-ninja: passed
ctest --preset package-release-clang-ninja --output-on-failure: passed, 162/162
cmake --build --preset package-release-clang-ninja --target package: passed
package artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Latest exact-SHA GitHub cross-compiler validation run `26132278315` passed for `8ae7d596facc59873dc75a31d5c23cdc8cf06763`:

```text
workflow:           Cross Compiler Validation
run:                https://github.com/tonytranrp/Pipeline-c-/actions/runs/26132278315
validated SHA:      8ae7d596facc59873dc75a31d5c23cdc8cf06763
GCC C++20:          passed, 162/162, g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
GCC C++23:          passed, 162/162, g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Clang C++20:        passed, 162/162, Ubuntu clang version 18.1.3 (1ubuntu1)
Clang C++23:        passed, 162/162, Ubuntu clang version 18.1.3 (1ubuntu1)
MSVC C++20:         passed, 161/161, Visual Studio 2022 Enterprise, MSVC 19.44.35226
Package release:    passed, 162/162, package TGZ generated
CMake:              3.31.6 in all GitHub Actions lanes
Ninja:              1.11.1 in GitHub Actions Linux lanes
package artifact:   /home/runner/work/Pipeline-c-/Pipeline-c-/build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Normal CI also passed on the same SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26132264545>.

See [Cross-Compiler Validation Status](cross-compiler-validation.md) for the detailed matrix and workflow notes.

## PR summary draft

- Hardened the explicit sequential fan-in slice with failed-case aggregation, void-output case aggregation, borrowed move-only fan-in, and case-failure observer/trace coverage while preserving selected-output branch/join behavior; backend/parallel fan-in joins and backend cancellation/scheduling policies remain out of scope.
- Hardened DOT/JSON helper output for supported linear and selected-output branch shapes, including branch identity metadata and JSON/DOT escaping coverage.
- Added developer-preset compile-time/header benchmark smoke targets and the aggregate `pb_compile_time_benchmarks` target; no timing thresholds are enforced.
- Converted Release smoke tests from assert-only checks to explicit aborting test helpers, keeping Release/NDEBUG checks meaningful and warning-clean.
- Verified the validated code SHA with local developer/package presets, GitHub cross-compiler validation, and normal CI.

## Release note guardrail

Release notes may mention the GitHub cross-compiler matrix pass only with exact code SHA `8ae7d596facc59873dc75a31d5c23cdc8cf06763` and workflow link <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26132278315>. If later non-doc code changes land, rerun the cross-compiler workflow and update this page before tagging.
