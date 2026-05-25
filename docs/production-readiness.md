# Production Readiness Status

This project is still an MVP foundation for the roadmap in `research/pipeline_builder_cpp_research_plan.md`. Treat the current tree as production-readiness scaffolding rather than a stable release.

## Supported today

- C++20 target-based CMake project with `pb::core`, `pb::runtime`, and the `pb::pipeline` compatibility target.
- Standard-library-only public core/runtime surfaces, including the sequential backend and the first `pb::runtime::thread_pool_backend` fan-in backend slice.
- Linear typed pipeline validation with `pb::from<T>::then<S>::to<U>`.
- Stage metadata through explicit `input_type`, `output_type`, `error_type`, and `stage_name()` or adapter-provided traits.
- Public pipeline introspection helpers: `pipeline_size_v`, `pipeline_input_t`, `pipeline_output_t`, `pipeline_stage_t`, `pipeline_stage_descriptor_t`, `describe()`, stage records, and linear edge records.
- Free-function and function-object adapters for legacy code integration.
- Sequential runtime execution for validated linear pipelines, including zero-stage identity pipelines.
- Sequential branch execution with optional selected-output join stages, observer case events, stateful branch storage, branch-child fallback identities, heterogeneous branch outputs represented as `std::variant`, selected-output type-list joins, and move-only selected-branch input consumption when predicates observe by `const input_type&`. Variant joins preserve duplicate output alternatives by index. Type-list selected-output joins dispatch by C++ type; duplicate same-type branch outputs share the same overload unless the user encodes case identity into the output type. Explicit sequential fan-in joins are supported through `::fan_in<JoinStage>` / `::join_all<JoinStage>` with ordered `pb::fan_in_results<...>` aggregates for zero/one/many passing cases, failed predicate/stage slots with diagnostics, void-output case aggregation, and borrowed move-only fan-in when every passing case stage accepts `const input_type&`. Compile-fail coverage still rejects predicates that consume move-only inputs and fan-in stages that would require copying a non-copyable input by value.
- `pb::shared_view<T>` — copy-cheap `shared_ptr`-backed wrapper for non-copyable fan-in inputs; `pb::make_shared_view` convenience factory; `pb::fan_in_uses_copy_v` / `pb::fan_in_uses_borrow_v` introspection variable templates. Header: `include/pb/runtime/clone.hpp`. Test: `tests/runtime/fan_in_shared_view_smoke.cpp`.
- `pb::projected<From, Projection, Stage>` — projection adapter wrapping an existing stage with a user-supplied projection callable, enabling plug-in borrow/extract strategies for fan-in branches and linear chains without rewriting the underlying stage. Same header. Test: `tests/runtime/projected_stage_smoke.cpp`.
- Error-policy DSL: five factory-built, engine-side wrappers — `pb::with_throw_on_error` → `throwing_engine` (throws `pb::pipeline_exception` on failure), `pb::with_terminate_on_error` → `terminating_engine` (calls `std::terminate()` on failure), `pb::with_ignore_errors` → `ignoring_engine` (returns user-supplied fallback on failure), `pb::with_propagate_exceptions` → `propagating_engine` (re-throws exceptions captured as `error_category::exception`), `pb::with_verbose_diagnostics` → `verbose_engine` (owns a `pb::verbose_observer` that logs every transition to a `std::ostream` under the `[pb.verbose]` prefix). Header: `include/pb/runtime/error_policy.hpp`. Tests: `tests/runtime/throw_on_error_smoke.cpp`, `tests/runtime/policy_dsl_engine_smoke.cpp`. Note: `run()` / `try_run()` policy parity across all wrappers and all topology combinations is still being hardened.
- `pb.core.graph.v1` typed schema-version contract: the literal `"pb.core.graph.v1"` identifier is enforced by compile-time regression in `tests/compile_pass/schema_v1_contract.cpp` and byte-equal golden output is locked in `tests/compile_pass/schema_v1_extended_golden.cpp`. Schema specification: `docs/export-schema-v1.md`.
- `pb::export_text` — compact text-format export helper alongside the existing JSON/DOT helpers. Header: `include/pb/core/export_text.hpp`. Tests: `tests/compile_pass/export_text.cpp`, `tests/compile_pass/export_text_golden.cpp`, `tests/compile_pass/public_headers/export_text.cpp`.
- Typed C++26 / C++23 feature gate constants: `pb::features::has_cpp26`, `pb::features::has_reflection`, `pb::features::has_contracts`, `pb::features::has_pack_indexing`, `pb::features::has_std_expected`, `pb::features::has_deducing_this` as `inline constexpr bool`; all evaluate to `false` on the C++20 baseline. Implication invariants (`reflection_implies_cpp26`, `contracts_implies_cpp26`) also exported. Header: `include/pb/core/cpp26_features.hpp`. Docs: `docs/cpp26-feature-gates.md`. Test: `tests/compile_pass/cpp26_features_fallback.cpp`.
- Reflection adapter scaffold in `include/pb/adapt/reflect.hpp` gated on `PB_HAS_REFLECTION` with a graceful no-op C++20 fallback. C++26 reflection is not supported on current baselines — the scaffold is a forward-compat seam only. Test: `tests/compile_pass/reflect_stage_gate.cpp`.
- Substantially expanded `pb_cli` (`tools/pb_cli.cpp`, +389 lines) with `tests/run_pb_cli_describe.cmake` test driver.
- Compile-pass, compile-fail, runtime, example, package, compile-time/header benchmark smoke scaffolding, and cross-compiler validation workflow. Targeted thread-pool backend fan-in runtime coverage is present. Current working tree: **185/185 local ctest**; cross-compiler validation on the new SHA pending.

## Current release gate status

Last cross-compiler-validated code snapshot: code SHA `87299c14c813753d170911239e251064cbbfee6f` (`Support the first thread-pool fan-in backend slice`) passed the local developer and package-release verification loop on 2026-05-20: `git diff --check`, `clang-dev-ninja` build/full CTest `163/163`, targeted backend/policy/export/descriptor CTest `33/33`, `package-release-clang-ninja` build/CTest `163/163`, and TGZ package generation (`build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`).

Latest GitHub cross-compiler validation snapshot: code SHA `87299c14c813753d170911239e251064cbbfee6f` passed the cross-compiler workflow on 2026-05-20: GCC C++20/C++23 `163/163`, Clang C++20/C++23 `163/163`, MSVC C++20 `163/163`, and clean Ubuntu `package-release-clang-ninja` `163/163` plus TGZ package generation. The matching normal CI workflow also passed the Clang dev, package-release, and benchmark-smoke lanes. See [Cross-Compiler Validation Status](cross-compiler-validation.md).

Current working tree state: **185/185 local ctest** (includes the new error-policy DSL, `pb::shared_view`, `pb::projected`, schema v1 contract, `pb::export_text`, typed C++26 feature gates, reflection adapter scaffold, and expanded `pb_cli` batches). Cross-compiler validation on the new working-tree SHA has not yet been rerun. Rerun the cross-compiler workflow on the final candidate SHA before any release-facing claim.

The package-consumer smoke gate now uses the target-specific downstream executables generated by `cmake/PBPackageSmoke.cmake.in`: `pipebuilder_core_target_smoke`, `pipebuilder_runtime_target_smoke`, and `pipebuilder_pipeline_alias_smoke`. Treat `pb_package_config_smoke` as the minimum package-verification gate, and require a fresh `package-release-clang-ninja` configure/build/CTest/package run on the candidate SHA before calling the package lane release-ready.

## Historical local MVP audit

Baseline `aa3b8f6` on `main` was audited against `research/pipeline_builder_cpp_research_plan.md` as a linear MVP foundation. The developer preset configure/build/CTest loop passed with **78/78 tests**. That historical audit supports the linear MVP claims below, but later candidate evidence is required for newer branch/join support and any roadmap-only items such as full graph export, optional backends, stable runtime descriptors, or release-grade benchmark budgets.

See `continue.md` for the resume checklist and next 3-agent long-horizon work queue.

## Current branch-execution update

Post-`aa3b8f6` branch hardening promoted sequential branch execution into the supported surface. Current branch/join evidence includes public `::branch<...>`, selected-output `::join<...>`, and explicit fan-in `::fan_in<...>` / `::join_all<...>` DSL support; homogeneous branch-output validation; explicit unified-output validation for variant execution output; heterogeneous outputs via `std::variant`; unified-output join validation plus selected-output type-list join dispatch with raw output type-list metadata retained; first-match-wins selected-output routing; all-passing sequential fan-in aggregation with skipped/completed/failed case states; void-output fan-in cases; borrowed move-only fan-in when all case stages accept `const input_type&`; const-input predicate evaluation in per-run and stateful execution; move-only selected-branch input consumption plus bad-predicate negative coverage; observer case events including fan-in case failure; stateful branch predicate/stage storage; branch runtime tests; branch examples; and targeted branch export compile-pass checks. Thread-pool backend fan-in now has a first implementation slice with deterministic case-index aggregation, drain-running-cases behavior, and synchronized observer forwarding. oneTBB/Taskflow/stdexec backends, preemptive cancellation, broad backend branch examples/benchmarks, and stable external descriptor/export compatibility remain roadmap-only until their own implementation and verification slices land.

### Current production-grade boundary table

| Surface | Production-grade for current scope | Partial / not production-grade yet |
| --- | --- | --- |
| Linear DSL/runtime | Typed linear DSL, compile-time validation, sequential runtime, result/expected-like handling, adapters, examples, and package smoke are tested and documented. | Stable observer ABI and richer topology coverage are still future work. |
| Branch/join | Selected-output branch/join, heterogeneous `std::variant` outputs, type-list selected-output joins, explicit sequential fan-in, failed/void fan-in slots, borrowed move-only fan-in, `pb::shared_view<T>` non-copyable fan-in inputs, `pb::projected` projection adapters, stateful branch storage, and selected-branch move-only consumption are supported. | Preemptive cancellation, dependency backend branch execution, selected-output tagged case-result policy, and broader ownership/lifetime policies remain roadmap-only. |
| Error-policy DSL | Five named wrappers shipped: `throwing_engine`, `terminating_engine`, `ignoring_engine`, `propagating_engine`, `verbose_engine` (with owned `verbose_observer`). `pb::pipeline_exception` carries the runtime diagnostic. Smoke-tested. | `run()` / `try_run()` policy parity across all five wrappers and all topology combinations is still being hardened. Full policy-surface docs are pending. |
| Backend execution | `sequential` plus the standard-library `thread_pool_backend` fan-in slice are supported and validated on the last cross-compiler SHA. | oneTBB, Taskflow, stdexec, broad backend examples/benchmarks, and backend cancellation/error policy remain incomplete. |
| Descriptor/export | DOT/JSON/text helpers cover linear, selected-output branch, and explicit fan-in helper output with golden tests. `pb.core.graph.v1` typed contract pins the schema-version string and field layout at compile time. `pb::export_text` ships alongside the JSON/DOT helpers. | A frozen external interchange schema, CLI/file export, backend graph export, and schema migration policy remain roadmap-only. |
| C++26 feature gates | `pb::features::*` typed `constexpr bool` constants for six language facilities plus implication invariants. All evaluate to `false` on C++20 baselines; fallback path is regression-tested. | C++26 reflection is gated and falls back — it is not supported on current compiler baselines. |
| Reflection adapter | `include/pb/adapt/reflect.hpp` scaffold compiles on C++20 with a graceful no-op fallback when `PB_HAS_REFLECTION` is absent. | No reflection-powered functionality ships until a C++26 reflection toolchain baseline is established. |
| Release evidence | Working tree at 185/185 local ctest. Last cross-compiler-validated SHA has local full/package validation plus GitHub CI and cross-compiler/package evidence. | Cross-compiler workflow not yet rerun on the new SHA. No published tag/release yet; MSVC C++23, Windows package-release, and benchmark budgets remain unclaimed. |

## Local readiness checks

Run the developer build and test loop before relying on the tree:

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja --output-on-failure
```

When touching examples or benchmark guidance, also run the relevant smoke targets from [Build and Verification](build.md). For compile-time/header scaffolding, use `cmake --build --preset clang-dev-ninja --target pb_compile_time_benchmarks` plus `ctest --preset clang-dev-ninja --output-on-failure -R 'benchmark|compile_time|header|bench'`. Record the preset, compiler, and build type with any benchmark timing because the benchmark targets are not release performance gates yet.

## Package consumer smoke

The installed package is expected to support downstream CMake consumers that use:

```cmake
find_package(pipebuilder CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE pb::pipeline)
```

`pb::pipeline` remains the documented compatibility target for consumers that want the combined surface. The installed package also exports `pb::core` and `pb::runtime` for consumers that need the split targets directly, and `pb::pipeline` forwards to `pb::runtime`.

The package release preset in [Build and Verification](build.md) is the release-readiness gate for this contract. The `pb_package_config_smoke` test installs the current build into a temporary prefix, verifies that `find_package(pipebuilder CONFIG REQUIRED)` defines `pb::core`, `pb::runtime`, and `pb::pipeline`, builds and runs separate consumers against each imported target, and checks the generated TGZ for key package entries. Treat that test as the minimum package-consumer contract only when it passes on the release candidate SHA.

## Release checklist

Before cutting a release candidate, collect evidence for:

- `cmake --list-presets=all` succeeds and the preset names are unique.
- The debug package preset and release Clang package preset both configure, build, and pass CTest on the candidate SHA.
- `pb_package_config_smoke` passes in the release package preset and its target-specific downstream consumers run successfully.
- The release package target produces a local archive for inspection before any external publication.
- Benchmark smoke targets run, and any recorded numbers include the context listed in [Build and Verification](build.md).
- Public examples still build/run, including the intentional compile-fail diagnostic example.
- Known gaps below are either documented for the release or converted into blocking issues.

## Known gaps before a stable release

- Branch/join marker aliases plus `branch_case_output` / `branch_outputs` marker metadata and unsupported-boundary, branch source/predicate, homogeneous branch-node case-input, branch output compatibility validation, join consumption validation, invalid join-stage, and branch-output marker misuse diagnostics now exist. Sequential selected-output branch execution with optional join stages, observer events, stateful storage, heterogeneous outputs through `std::variant` plus type-list selected-output joins including duplicate output alternatives by index, move-only selected-branch input consumption, and explicit sequential fan-in joins are supported with runtime tests, examples, and compile-time join validation. Thread-pool backend fan-in has targeted runtime support and tests for parallel case execution, deterministic aggregation, failed-case aggregation, void-output aggregation, and borrowed move-only input. oneTBB/Taskflow/stdexec backends, preemptive cancellation, broad backend branch examples/benchmarks, and a stable external runtime descriptor/export contract remain roadmap, not current guarantees. See [Research Verification Matrix](research-verification-matrix.md), [Branch / Join Roadmap / Status](branch-join-roadmap.md), [Graph Export Roadmap / Status](graph-export-roadmap.md), [Observer Hooks Roadmap / Status](observer-hooks-roadmap.md), [Optional Backends Roadmap / Status](optional-backends-roadmap.md), and [Runtime Descriptor Roadmap / Status](runtime-descriptor-roadmap.md) for current boundaries.
- The active missing-feature queue is tracked in [Roadmap Gap Map](roadmap-gap-map.md#active-missing-feature-priority-queue). Treat worker-lane reports as integration leads, not release claims, until the corresponding commits, tests, and docs are present on the candidate branch.
- Public diagnostics are covered by compile-fail smoke tests, but the exact diagnostic wording is still being hardened. See [Diagnostics Roadmap / Status](diagnostics-roadmap.md) for the current supported boundary versus the richer roadmap.
- Runtime error propagation, `error_record` / `to_record(...)` diagnostic projection, observer callbacks, custom expected-like diagnostic stage identity, and linear descriptor/observer/error identity checks exist for the current sequential path; richer exception policies, `std::expected` integration, exported diagnostic artifacts, and fully stable observer contracts are future slices. Zero-stage identity pipelines have no stage callbacks because no stage executes.
- The error-policy DSL is shipped: five factory-built wrappers (`throwing_engine`, `terminating_engine`, `ignoring_engine`, `propagating_engine`, `verbose_engine`) are tested in `tests/runtime/throw_on_error_smoke.cpp` and `tests/runtime/policy_dsl_engine_smoke.cpp`. `run()` / `try_run()` policy parity across all five wrappers and all supported topologies is still being hardened.
- DOT/JSON/text export helpers now cover linear, selected-output branch, and explicit fan-in pipelines and branch-aware helper output is descriptor-record-backed, including branch topology in JSON, branch case identity fields, DOT label escaping, JSON string escaping, and helper-output golden regression tests. The `pb.core.graph.v1` schema-version string is now backed by a typed contract (`tests/compile_pass/schema_v1_contract.cpp`) that enforces the literal identifier at compile time; byte-equal golden output is locked in `tests/compile_pass/schema_v1_extended_golden.cpp`. `pb::export_text` adds a compact text-format helper. A frozen external interchange schema, CLI/file export, backend graph export, and schema migration policy remain roadmap-only.
- Optional backend support now includes the standard-library `thread_pool_backend` for the current fan-in slice. The backend feature matrix marks sequential and thread_pool as supported, while oneTBB, Taskflow, and stdexec remain roadmap/experimental.
- Compile-time/header benchmark smoke targets can prove the representative header, 5-stage, and 50-stage translation units build and run from the developer preset, but release timing thresholds and CI regression budgets are not established.
- Local and GitHub validation passed for code SHA `87299c14c813753d170911239e251064cbbfee6f`: local `clang-dev-ninja` and `package-release-clang-ninja` each passed `163/163` plus package generation, normal CI passed Clang dev/package/benchmark lanes, and GitHub cross-compiler validation passed GCC C++20/C++23, Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release. The current working tree passes **185/185 local ctest**; cross-compiler validation on the new SHA is pending. MSVC C++23, Windows package-release, C++26 gates, and other platform/package-manager matrices remain unclaimed.

## Small production-readiness slice (safe, immediate)

For the next minimal safe release-hardening step, refresh exact-SHA release evidence on the final candidate, then archive the result:

1. `cmake --preset package-release-clang-ninja`
2. `cmake --build --preset package-release-clang-ninja`
3. `ctest --preset package-release-clang-ninja --output-on-failure -R pb_package_config_smoke`
4. `cmake --build --preset package-release-clang-ninja --target package`
5. Run or rerun `Cross Compiler Validation` if the final candidate includes non-doc code changes after the latest validation snapshot.
6. Record the package smoke pass, package artifact path, cross-compiler workflow URL, preset, compiler versions, and commit SHA.

This keeps the slice narrow, deterministic, and low-risk: it validates install-time consumer compatibility plus compiler-matrix portability without changing runtime behavior.

## Documentation contract

Examples should show one complete successful path and one intentional failure path. If an example relies on future roadmap behavior, label it as aspirational instead of presenting it as supported. Keep public API promises aligned with the tests that currently pass.
