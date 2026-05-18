# Cross-Compiler Validation Status

This page records the latest validation-only pass for the current code surface. It is evidence, not a feature claim: roadmap-only items stay roadmap-only even when the existing tests pass across compilers.

## Latest completed validation run

```text
validated_code_sha=6805543ede6946aa283be7f24fb3736c762f47b2
date_utc=2026-05-18
workflow=Cross Compiler Validation
run=https://github.com/tonytranrp/Pipeline-c-/actions/runs/26058848575
result=PASS
```

The workflow was added specifically for validation coverage. It configures, builds, and runs CTest for Linux GCC/Clang C++20 and C++23, validates MSVC C++20 on Windows, and runs the package-release preset in a clean Ubuntu job. The docs-only commits after this SHA do not change library code, but a release tag should still rerun the workflow on the final tag candidate if strict exact-SHA release evidence is required.

## Matrix results

| Lane | Environment | Result | Test summary | Compiler/tool evidence |
| --- | --- | --- | --- | --- |
| GCC C++20 | Ubuntu GitHub Actions, Ninja Debug | PASS | `150/150` | `g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`, CMake `3.31.6`, Ninja `1.13.2` |
| GCC C++23 | Ubuntu GitHub Actions, Ninja Debug | PASS | `150/150` | `g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0`, CMake `3.31.6`, Ninja `1.13.2` |
| Clang C++20 | Ubuntu GitHub Actions, Ninja Debug | PASS | `150/150` | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2` |
| Clang C++23 | Ubuntu GitHub Actions, Ninja Debug | PASS | `150/150` | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2` |
| MSVC C++20 | Windows GitHub Actions, Visual Studio 2022 Debug | PASS | `149/149` | Visual Studio 2022 Enterprise, MSVC `19.44.35226`, CMake `3.31.6` |
| Package release clean Ubuntu | `package-release-clang-ninja` preset | PASS | `150/150` + TGZ package generated | `Ubuntu clang version 18.1.3 (1ubuntu1)`, CMake `3.31.6`, Ninja `1.13.2`; generated `pipebuilder-0.1.0-Linux.tar.gz` |

The normal CI workflow also passed on the same SHA:

```text
workflow=CI
run=https://github.com/tonytranrp/Pipeline-c-/actions/runs/26058841298
result=PASS
jobs=Clang dev preset, Package release preset, Benchmark smoke preset
```

## Validation boundaries

This pass supports claims that the current tested surface configures, builds, and passes its test suite on the matrix above. It does **not** mean these roadmap items are implemented:

- `type_list` / true multi-input join execution
- descriptor-backed stable DOT/JSON graph export
- CLI/file export of user pipeline definitions
- thread-pool / oneTBB / Taskflow / stdexec pipeline backends
- C++ modules
- C++26 reflection/contracts integrations
- benchmark thresholds or CI-enforced performance budgets
- stable/frozen diagnostic wording across all future features

## Known workflow limitation

The MSVC validation job disables `PB_ENABLE_PACKAGE_SMOKE` because it is a Debug configure/build/CTest lane. Package installation/export/archive behavior is validated separately by the clean Ubuntu `package-release-clang-ninja` job. Keep this split unless a Windows package-release lane is added.

GitHub currently emits a Node 20 deprecation warning for `actions/checkout@v4`; this is not a test failure, but future workflow maintenance should update the action/runtime when required by GitHub-hosted runners.
