# Current PR / Release Summary

Status snapshot for the current branch/export hardening and validation batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Latest code-validation SHA: `6805543ede6946aa283be7f24fb3736c762f47b2` on `main` / `origin/main`.
- Cross-compiler validation workflow: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26058848575> — **passed**.
- Normal CI workflow on the same SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26058841298> — **passed**.
- This docs update may create a later docs-only SHA. If a release tag requires exact-SHA evidence, rerun the workflows on the final tag candidate; no library code changed in this docs pass.

## What can be claimed with current evidence

- Linear typed pipeline validation and sequential runtime execution remain supported.
- Public branch/join DSL is supported for the sequential runtime slice.
- Homogeneous branch outputs and variant-based heterogeneous branch outputs are supported, including duplicate output alternatives preserved by `std::variant` index.
- Join validation checks the unified branch execution output, while raw branch output type-list metadata remains available for introspection.
- Move-only selected-branch input consumption is supported when predicates inspect by `const input_type&`; consuming predicates for move-only inputs remain unsupported and have negative compile-fail coverage.
- Stateful branch predicates/stages are covered under `pb::runtime::stateful_sequential`, and predicate invocation uses const-input semantics in both per-run and stateful paths.
- DOT/JSON helpers cover linear and supported branch pipelines, including JSON branch topology detection, DOT label escaping, and helper-output golden regressions.
- `pb::runtime::thread_pool` is a standalone utility only; it is not a pipeline backend.

## What must stay roadmap-only

- `type_list` / true multi-input join execution.
- Descriptor-backed stable DOT/JSON graph export schemas and release-grade compatibility fixtures.
- CLI/file export for user pipeline definitions.
- Thread-pool, oneTBB, Taskflow, or stdexec pipeline executor backends.
- C++ modules and C++26 reflection/contracts feature integrations.
- Stable/frozen diagnostic wording across all future features.
- Benchmark thresholds, dashboards, or CI-enforced compile-time/runtime performance budgets.

## Validation evidence collected

Cross-compiler validation run `26058848575` passed with these lanes:

```text
GCC C++20:        passed, 150/150, g++ 13.3.0
GCC C++23:        passed, 150/150, g++ 13.3.0
Clang C++20:      passed, 150/150, Ubuntu clang 18.1.3
Clang C++23:      passed, 150/150, Ubuntu clang 18.1.3
MSVC C++20:       passed, 149/149, MSVC 19.44.35226 / VS 2022 Enterprise
Package release:  passed, 150/150, package TGZ generated
CMake:            3.31.6 in GitHub Actions lanes
Ninja:            1.13.2 in GitHub Actions Linux lanes
```

Package-release clean Ubuntu evidence from the same run:

```text
cmake --preset package-release-clang-ninja: passed
cmake --build --preset package-release-clang-ninja: passed
ctest --preset package-release-clang-ninja --output-on-failure: passed, 150/150
cmake --build --preset package-release-clang-ninja --target package: passed
artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

See [Cross-Compiler Validation Status](cross-compiler-validation.md) for the detailed matrix and workflow notes.

## PR summary draft

- Added/validated a cross-compiler workflow covering GCC C++20/C++23, Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release.
- Preserved the supported branch/export boundary: sequential branch execution, variant-based heterogeneous output, move-only selected-stage consumption, and helper-level DOT/JSON export.
- Kept descriptor-backed graph export, optional executor backends, C++ modules/C++26 integrations, and performance budgets out of the supported claims.
- Fixed MSVC validation portability in the workflow by running CTest under the MSVC compiler environment and keeping package smoke in the dedicated package-release lane.

## Release note guardrail

Release notes may mention the matrix pass only with the exact SHA and workflow link above. If more non-doc code changes land before tagging, rerun the cross-compiler workflow and update this page plus `docs/cross-compiler-validation.md`.
