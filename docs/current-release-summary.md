# Current PR / Release Summary

Status snapshot for the current branch/export, compile-time benchmark, sequential fan-in, and validation batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Current local HEAD: this fan-in commit (`Add explicit fan-in to close the branch graph gap`).
- Current branch: `main`; push/cross-compiler validation still needs to run for this exact SHA.
- Local fan-in candidate verification: Clang developer configure/build/full CTest `162/162`, GCC/default developer configure/build/full CTest `162/162`, targeted fan-in CTest `6/6` including non-copyable-input and void-output boundary diagnostics, package-release configure/build/CTest `162/162`, and package artifact `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`.
- Latest GitHub cross-compiler validation SHA: `f56fa54399a7a6a4f1dd55433634f13aee9c3174`. Workflow: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329> — **passed**.
- Normal CI workflow for that cross-compiler SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26069390429> — **passed**.
- Release guardrail: rerun GitHub cross-compiler validation on this final SHA because the latest GitHub validation predates these non-doc hardening changes.

## What can be claimed with current evidence

- Linear typed pipeline validation and sequential runtime execution remain supported.
- Public branch/join DSL is supported for the selected-output sequential runtime slice.
- Homogeneous branch outputs and variant-based heterogeneous branch outputs are supported, including duplicate output alternatives preserved by `std::variant` index.
- Type-list selected-output joins dispatch by C++ type; duplicate same-type branch outputs share the same overload unless the user encodes case identity into the output type.
- Move-only selected-branch input consumption is supported when predicates inspect by `const input_type&`; consuming predicates for move-only inputs remain unsupported and have negative compile-fail coverage.
- Explicit sequential fan-in joins are supported through `::fan_in<JoinStage>` / `::join_all<JoinStage>`; all predicates are evaluated, all passing cases run in declaration order, zero passing cases are valid, duplicate same-type outputs are distinguished by case index in `pb::fan_in_case_result<I, T>`, and backend/parallel fan-in remains roadmap-only.
- Stateful branch predicates/stages are covered under `pb::runtime::stateful_sequential`, and predicate invocation uses const-input semantics in both per-run and stateful paths.
- DOT/JSON helpers cover linear and selected-output branch pipelines, including descriptor-record-backed branch helper rendering, JSON branch topology detection, current top-level JSON fields (`schema_version`, `topology`, `stage_count`, `edge_count`, `stages`, `edges`), branch case identity fields, DOT label escaping, JSON string escaping, and helper-output golden regressions for `pb.core.graph.v1`.
- Compile-time/header smoke targets cover public header inclusion plus representative 5-stage and 50-stage chains through the `pb_compile_time_benchmarks` aggregate target.
- `pb::runtime::thread_pool` is a standalone utility only; it is not a pipeline backend.

## What must stay roadmap-only

- Backend/parallel fan-in / true backend multi-input join execution beyond the current sequential fan-in join model, plus richer fan-in failure aggregation.
- Stable descriptor/export schemas and release-grade compatibility fixtures beyond the current descriptor-record-backed helper output.
- CLI/file export as a stable public contract for user pipeline definitions.
- Thread-pool, oneTBB, Taskflow, or stdexec pipeline executor backends.
- Full policy DSL for error, exception, state, reference lifetime, diagnostic verbosity, and executor capability behavior.
- C++ modules and C++26 reflection/contracts feature integrations.
- Stable/frozen diagnostic wording across all future features.
- Benchmark thresholds, dashboards, release timing baselines, or CI-enforced compile-time/runtime performance budgets.

## Validation evidence collected

Current local verification for this fan-in commit:

```text
git diff --check: passed
cmake --preset clang-dev-ninja: passed
cmake --build --preset clang-dev-ninja --target pb_branch_fan_in_compile_pass pb_runtime_sequential_branch_fan_in: passed
ctest --preset clang-dev-ninja --output-on-failure -R 'fan_in|fan-in|branch_fan|sequential_branch_fan': passed, 6/6
cmake --build --preset clang-dev-ninja: passed
ctest --preset clang-dev-ninja --output-on-failure: passed, 162/162
cmake --fresh --preset dev-ninja: passed with GCC 13.3.0
cmake --build --preset dev-ninja: passed
ctest --preset dev-ninja --output-on-failure: passed, 162/162
cmake --preset package-release-clang-ninja: passed
cmake --build --preset package-release-clang-ninja: passed
ctest --preset package-release-clang-ninja --output-on-failure: passed, 162/162
cmake --build --preset package-release-clang-ninja --target package: passed
package artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Latest exact-SHA GitHub cross-compiler validation run `26070113329` passed for `f56fa54399a7a6a4f1dd55433634f13aee9c3174`:

```text
workflow:           Cross Compiler Validation
run:                https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329
validated SHA:      f56fa54399a7a6a4f1dd55433634f13aee9c3174
GCC C++20:          passed, 153/153, g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
GCC C++23:          passed, 153/153, g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
Clang C++20:        passed, 153/153, Ubuntu clang version 18.1.3 (1ubuntu1)
Clang C++23:        passed, 153/153, Ubuntu clang version 18.1.3 (1ubuntu1)
MSVC C++20:         passed, 152/152, Visual Studio 2022 Enterprise, MSVC 19.44.35226
Package release:    passed, 153/153, package TGZ generated
CMake:              3.31.6 in all GitHub Actions lanes
Ninja:              1.13.2 in GitHub Actions Linux lanes
package artifact:   /home/runner/work/Pipeline-c-/Pipeline-c-/build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Normal CI also passed on the same SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26069390429>.

See [Cross-Compiler Validation Status](cross-compiler-validation.md) for the detailed matrix and workflow notes.

## PR summary draft

- Hardened DOT/JSON helper output for supported linear and selected-output branch shapes, including branch identity metadata and JSON/DOT escaping coverage.
- Added developer-preset compile-time/header benchmark smoke targets and the aggregate `pb_compile_time_benchmarks` target; no timing thresholds are enforced.
- Added the first explicit sequential fan-in slice while preserving selected-output branch/join behavior; backend/parallel fan-in joins and richer failure aggregation remain out of scope.
- Kept stable descriptor/export schema, CLI/file export, optional backends, and release-grade performance budgets as roadmap-only work.
- Verified the current local HEAD with developer and package-release presets; GitHub cross-compiler validation still needs an exact final-SHA rerun.

## Release note guardrail

Release notes may mention the GitHub cross-compiler matrix pass only with exact SHA `f56fa54399a7a6a4f1dd55433634f13aee9c3174` and workflow link <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329>. For the current fan-in commit, release notes may mention only the local verification listed above until the cross-compiler workflow is rerun on the final SHA.
