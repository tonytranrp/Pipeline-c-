# Current PR / Release Summary

Status snapshot for the current branch/export, compile-time benchmark, and validation batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Current local HEAD: `4d7127c947213c30f994ad39a06b8f2119687596` (`Harden helper exports and compile-time smoke evidence`).
- Current branch: `main` / `origin/main` after push.
- Local current-HEAD verification: developer configure/build/full CTest `156/156`, targeted branch/export/descriptor/benchmark CTest `59/59`, compile-time benchmark aggregate target, package-release configure/build/CTest `156/156`, and package artifact `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`.
- Latest GitHub cross-compiler validation SHA: `f56fa54399a7a6a4f1dd55433634f13aee9c3174`. Workflow: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329> — **passed**.
- Normal CI workflow for that cross-compiler SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26069390429> — **passed**.
- Release guardrail: rerun GitHub cross-compiler validation on the final tag SHA because current HEAD contains later non-doc hardening changes.

## What can be claimed with current evidence

- Linear typed pipeline validation and sequential runtime execution remain supported.
- Public branch/join DSL is supported for the selected-output sequential runtime slice.
- Homogeneous branch outputs and variant-based heterogeneous branch outputs are supported, including duplicate output alternatives preserved by `std::variant` index.
- Type-list selected-output joins dispatch by C++ type; duplicate same-type branch outputs share the same overload unless the user encodes case identity into the output type.
- Move-only selected-branch input consumption is supported when predicates inspect by `const input_type&`; consuming predicates for move-only inputs remain unsupported and have negative compile-fail coverage.
- Stateful branch predicates/stages are covered under `pb::runtime::stateful_sequential`, and predicate invocation uses const-input semantics in both per-run and stateful paths.
- DOT/JSON helpers cover linear and selected-output branch pipelines, including descriptor-record-backed branch helper rendering, JSON branch topology detection, current top-level JSON fields (`schema_version`, `topology`, `stage_count`, `edge_count`, `stages`, `edges`), branch case identity fields, DOT label escaping, JSON string escaping, and helper-output golden regressions for `pb.core.graph.v1`.
- Compile-time/header smoke targets cover public header inclusion plus representative 5-stage and 50-stage chains through the `pb_compile_time_benchmarks` aggregate target.
- `pb::runtime::thread_pool` is a standalone utility only; it is not a pipeline backend.

## What must stay roadmap-only

- Parallel all-branches fan-in / true backend multi-input join execution beyond the selected-output sequential join model.
- Stable descriptor/export schemas and release-grade compatibility fixtures beyond the current descriptor-record-backed helper output.
- CLI/file export as a stable public contract for user pipeline definitions.
- Thread-pool, oneTBB, Taskflow, or stdexec pipeline executor backends.
- Full policy DSL for error, exception, state, reference lifetime, diagnostic verbosity, and executor capability behavior.
- C++ modules and C++26 reflection/contracts feature integrations.
- Stable/frozen diagnostic wording across all future features.
- Benchmark thresholds, dashboards, release timing baselines, or CI-enforced compile-time/runtime performance budgets.

## Validation evidence collected

Current local verification for `4d7127c947213c30f994ad39a06b8f2119687596`:

```text
git diff --check: passed
cmake --preset clang-dev-ninja: passed
cmake --build --preset clang-dev-ninja --target pb_compile_time_benchmarks: passed
cmake --build --preset clang-dev-ninja: passed
ctest --preset clang-dev-ninja --output-on-failure -R 'export|descriptor|branch|benchmark|compile_time|header|bench': passed, 59/59
ctest --preset clang-dev-ninja --output-on-failure: passed, 156/156
cmake --preset package-release-clang-ninja: passed
cmake --build --preset package-release-clang-ninja: passed
ctest --preset package-release-clang-ninja --output-on-failure: passed, 156/156
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
- Preserved selected-output branch/join boundaries and kept true all-branches fan-in/backend joins out of scope.
- Kept stable descriptor/export schema, CLI/file export, optional backends, and release-grade performance budgets as roadmap-only work.
- Verified the current local HEAD with developer and package-release presets; GitHub cross-compiler validation still needs an exact final-SHA rerun.

## Release note guardrail

Release notes may mention the GitHub cross-compiler matrix pass only with exact SHA `f56fa54399a7a6a4f1dd55433634f13aee9c3174` and workflow link <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329>. For current HEAD `4d7127c947213c30f994ad39a06b8f2119687596`, release notes may mention only the local verification listed above until the cross-compiler workflow is rerun on the final candidate SHA.
