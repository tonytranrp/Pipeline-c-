# Cross-Compiler Validation Status

This page records the latest completed GitHub cross-compiler pass. It is evidence, not a feature claim: roadmap-only items stay roadmap-only even when the existing tests pass across compilers. If local public-header or test changes land after the recorded SHA, rerun the workflow on the final candidate SHA before release tagging.

## Latest completed validation run

```text
validated_code_sha=f56fa54399a7a6a4f1dd55433634f13aee9c3174
date_utc=2026-05-19
workflow=Cross Compiler Validation
run=https://github.com/tonytranrp/Pipeline-c-/actions/runs/26070113329
result=PASS
```

The workflow was added specifically for validation coverage. It configures, builds, and runs CTest for Linux GCC/Clang C++20 and C++23, validates MSVC C++20 on Windows, and runs the package-release preset in a clean Ubuntu job. This run is the exact-SHA refresh for commit `f56fa54399a7a6a4f1dd55433634f13aee9c3174`; rerun it if later non-doc code changes land before release tagging.

## Matrix results

| Lane | Environment | Result | Test summary | Compiler/tool evidence |
| --- | --- | --- | --- | --- |
| GCC C++20 | Ubuntu GitHub Actions, Ninja Debug | PASS | `153/153` | `g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`, CMake `3.31.6`, Ninja `1.13.2` |
| GCC C++23 | Ubuntu GitHub Actions, Ninja Debug | PASS | `153/153` | `g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`, CMake `3.31.6`, Ninja `1.13.2` |
| Clang C++20 | Ubuntu GitHub Actions, Ninja Debug | PASS | `153/153` | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2` |
| Clang C++23 | Ubuntu GitHub Actions, Ninja Debug | PASS | `153/153` | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2` |
| MSVC C++20 | Windows GitHub Actions, Visual Studio 2022 Debug | PASS | `152/152` | Visual Studio 2022 Enterprise, MSVC `19.44.35226`, CMake `3.31.6` |
| Package release clean Ubuntu | `package-release-clang-ninja` preset | PASS | `153/153` + TGZ package generated | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2`; generated `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz` (`/home/runner/work/Pipeline-c-/Pipeline-c-/build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz` in the runner workspace) |

The normal CI workflow also passed on the same SHA:

```text
workflow=CI
run=https://github.com/tonytranrp/Pipeline-c-/actions/runs/26069390429
result=PASS
jobs=Clang dev preset, Package release preset, Benchmark smoke preset
```

## Validation boundaries

This pass supports claims that the current tested surface configures, builds, and passes its test suite on the matrix above. It does **not** mean these roadmap items are implemented:

- backend/parallel fan-in / true backend multi-input join execution beyond the current sequential fan-in slice
- stable descriptor/export compatibility beyond descriptor-record-backed helper DOT/JSON output
- CLI/file export of user pipeline definitions
- thread-pool / oneTBB / Taskflow / stdexec pipeline backends
- C++ modules
- C++26 reflection/contracts integrations
- benchmark thresholds or CI-enforced performance budgets
- stable/frozen diagnostic wording across all future features

## Known workflow limitation

The MSVC validation job disables `PB_ENABLE_PACKAGE_SMOKE` because it is a Debug configure/build/CTest lane. Package installation/export/archive behavior is validated separately by the clean Ubuntu `package-release-clang-ninja` job. Keep this split unless a Windows package-release lane is added.

GitHub currently emits a Node 20 deprecation warning for `actions/checkout@v4`; this is not a test failure, but future workflow maintenance should update the action/runtime when required by GitHub-hosted runners.
