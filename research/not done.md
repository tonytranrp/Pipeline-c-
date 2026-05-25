# Current implementation map against `pipeline_builder_cpp_research_plan.md`

This file tracks what is done, partially done, still missing, and what must be finished before Pipeline-c++ can claim the full original research-plan vision.

Updated after the production-grade batch that adds the typed `pb.core.graph.v1` schema-version contract, the five-wrapper error-policy DSL (`throwing`/`terminating`/`ignoring`/`propagating`/`verbose` engines), `pb::shared_view<T>` and `pb::projected<From, Projection, Stage>` fan-in clone/projection policies, the `pb::export_text` compact text helper, typed C++26 / C++23 feature gate constants under `pb::features::*`, the gated `pb::reflect_stage<T>` reflection-adapter scaffold, an expanded `pb_cli` (`list` / `describe` / `features`) with text/DOT/JSON output plus `--out=` file routing, and indexed I/O aliases (`pipeline_stage_input_t`, `pipeline_stage_output_t`) with named out-of-range diagnostics. Local working tree: **189/189 ctest** on `clang-dev-ninja`. The latest cross-compiler-validated SHA is still `87299c14c813753d170911239e251064cbbfee6f` — rerun the workflow on the new SHA before tagging.

## Latest evidence snapshot

Current local evidence for the working tree (HEAD `cd5a80c` plus the uncommitted production-grade batch + 3-agent wave):

```text
- cmake --preset clang-dev-ninja: passed
- cmake --build --preset clang-dev-ninja: passed (175/175 link targets)
- ctest --preset clang-dev-ninja --output-on-failure: passed, 189/189
```

The previous validated code SHA `87299c14c813753d170911239e251064cbbfee6f` still has fresh local + GitHub evidence:

```text
- ctest --preset clang-dev-ninja --output-on-failure: passed, 163/163
- ctest --preset package-release-clang-ninja --output-on-failure: passed, 163/163
- cmake --build --preset package-release-clang-ninja --target package: passed
- package artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Latest GitHub validation for that SHA (still current as the canonical cross-compiler reference until the new SHA is revalidated):

```text
- workflow: Cross Compiler Validation
- run: https://github.com/tonytranrp/Pipeline-c-/actions/runs/26145779030
- validated SHA: 87299c14c813753d170911239e251064cbbfee6f
- GCC C++20/C++23: passed, 163/163
- Clang C++20/C++23: passed, 163/163
- MSVC C++20: passed, 163/163
- clean Ubuntu package-release-clang-ninja: passed, 163/163 plus TGZ package generation
```

Normal CI also passed on the same SHA: <https://github.com/tonytranrp/Pipeline-c-/actions/runs/26145765932>.

## Done / production-grade for the current supported scope

These areas are implemented, tested, documented, and safe to present as supported when the release evidence is fresh.

| Area | Current status | Evidence / boundary |
| --- | --- | --- |
| CMake/package skeleton | Done for the current package shape. | `pb::core`, `pb::runtime`, and `pb::pipeline` install/export targets exist; package smoke validates downstream consumers and TGZ contents. |
| C++20 baseline | Done. | Target-based CMake requires C++20 and disables extensions. C++23 is validated in GitHub lanes, but C++20 remains the baseline. |
| Linear typed DSL | Done. | `pb::from<T>::then<S>::to<U>` and `::to<U>` are implemented with compile-pass and compile-fail coverage. |
| Stage traits and metadata | Done for current traits. | `input_type`, `output_type`, optional `error_type`, names, keys, public aliases, and descriptor helpers are covered. |
| Indexed pipeline I/O aliases | Done. | `pb::pipeline_stage_input_t<P, N>` and `pb::pipeline_stage_output_t<P, N>` ship with named out-of-range `static_assert` diagnostics; compile-pass + two compile-fail tests cover the alias and OOB boundary. |
| Linear compile-time validation | Done for current linear graph. | Invalid stages, adjacent edge mismatch, sink mismatch, and public-header coverage are tested. |
| Sequential runtime | Done for validated linear pipelines. | `pb::compile<Pipeline>(pb::runtime::sequential{})`, `run()`, `try_run()`, move-only linear paths, and zero-stage identity behavior are tested. |
| Result / expected-like plumbing | Done for current supported behavior. | `pb::runtime::result`, expected-like normalization, stage failure propagation, custom error conversion, and exception capture boundaries are tested. |
| Error-policy DSL | Done for the wrapper surface; cross-wrapper parity hardening still open. | Five factory-built engine wrappers under `include/pb/runtime/error_policy.hpp`: `pb::with_throw_on_error` → `throwing_engine`, `pb::with_terminate_on_error` → `terminating_engine`, `pb::with_ignore_errors` → `ignoring_engine`, `pb::with_propagate_exceptions` → `propagating_engine`, `pb::with_verbose_diagnostics` → `verbose_engine` (owns `pb::verbose_observer` writing `[pb.verbose]` events to a `std::ostream`). `pb::pipeline_exception::diagnostic()` exposes the underlying `pb::runtime::error`. Smoke coverage: `tests/runtime/throw_on_error_smoke.cpp`, `tests/runtime/policy_dsl_engine_smoke.cpp`, `tests/compile_pass/terminate_on_error_smoke.cpp`. |
| Observer hooks for sequential runtime | Done for current sequential hooks. | Stage start/success/failure/exception, branch case events, and the auto-attached `verbose_observer` from the policy DSL are covered. Stable ABI/event-schema guarantees remain future work. |
| User-code adapters | Done for current adapter shapes. | Free-function, member-function, and function-object/functor adapters are covered by compile-pass, compile-fail, runtime, docs, and examples. |
| Branch / selected-output join surface | Done for the supported sequential slice. | Public `::branch<...>` and `::join<...>`, homogeneous outputs, `std::variant` heterogeneous outputs, duplicate variant alternatives by index, selected-output type-list joins, observer case events, stateful storage, move-only selected-branch input consumption, examples, and tests are present. |
| Fan-in join surface | Done for sequential plus first thread-pool backend slice. | Public `::fan_in<JoinStage>` and `::join_all<JoinStage>` evaluate every predicate, run all passing cases in declaration order, continue after predicate/stage failures, and pass `pb::fan_in_results<pb::fan_in_case_result<I, T>...>` to the join stage. Covers zero/one/many passing cases, duplicate same-type outputs by index, stateful storage, void-output cases, copy-requiring non-copyable fan-in rejection, borrowed move-only fan-in, thread-pool parallel case overlap, deterministic aggregate ordering, and synchronized observer forwarding. |
| Fan-in clone / projection policies | Done. | `pb::shared_view<T>` (copy-cheap `shared_ptr`-backed wrapper) + `pb::make_shared_view` factory let non-copyable owned values flow through fan-in branches. `pb::projected<From, Projection, Stage>` wraps an existing stage with a user-supplied projection callable so the same stage participates in pipelines with different upstream payload types. `pb::fan_in_uses_copy_v` / `pb::fan_in_uses_borrow_v` expose the strategy at compile time. Header: `include/pb/runtime/clone.hpp`. Tests: `tests/runtime/fan_in_shared_view_smoke.cpp`, `tests/runtime/projected_stage_smoke.cpp`. |
| Branch case identity metadata | Done for helper/export preparation. | Descriptor branch case records carry deterministic case and node identities plus labels. Runtime fallback identities exist for unnamed branch children. |
| `pb.core.graph.v1` typed schema contract | Done as a regression-locked typed contract. | The literal `"pb.core.graph.v1"` identifier is enforced at compile time by `tests/compile_pass/schema_v1_contract.cpp`; byte-equal helper output for linear, branch, and fan-in pipelines is locked in `tests/compile_pass/schema_v1_extended_golden.cpp`. Formal spec: `docs/export-schema-v1.md`. This is a typed regression net, not yet a promised frozen external interchange schema. |
| DOT / JSON / text helper export | Done as a helper surface for supported shapes. | Linear, selected-output branch, and explicit fan-in helper output uses `schema_version = "pb.core.graph.v1"`, branch topology, branch cases, JSON escaping, DOT escaping, and golden tests. The compact `pb::export_text` helper ships alongside the JSON/DOT helpers (`include/pb/core/export_text.hpp`). Tests: `tests/compile_pass/export_text*.cpp`. |
| `pb_cli` introspection CLI | Done as a built-in introspection surface; broader CLI contract still future work. | `pb_cli list`, `pb_cli describe <name> --format=<dot\|json\|text> [--out=<path>]`, and `pb_cli features` are built into every preset. Three built-in pipelines ship: `order-linear`, `order-branch`, `order-fan-in`. CTest driver: `tests/run_pb_cli_describe.cmake`. Tool source: `tools/pb_cli.cpp` (+389 lines). |
| Typed C++26 / C++23 feature gates | Done as forward-compat gate constants with a regression-tested fallback. | `pb::features::has_cpp26`, `has_reflection`, `has_contracts`, `has_pack_indexing`, `has_std_expected`, `has_deducing_this` are `inline constexpr bool` constants. Implication invariants (`reflection_implies_cpp26`, `contracts_implies_cpp26`) are exported. All `PB_HAS_*` macros honour user pre-defines so downstream builds can force-disable a feature to test their fallback. Header: `include/pb/core/cpp26_features.hpp`. Docs: `docs/cpp26-feature-gates.md`. Test: `tests/compile_pass/cpp26_features_fallback.cpp`. |
| Reflection adapter scaffold | Done as a gated forward-compat seam — not a working reflection feature yet. | `pb::reflect_stage<T>` is declared on every compiler; when `PB_HAS_REFLECTION == 1` it emits a working reflection-driven adapter, on C++20 baselines instantiating it produces a named `static_assert` pointing at the gate. `pb::reflect_stage_available_v<T>` mirrors the macro. Header: `include/pb/adapt/reflect.hpp`. Test: `tests/compile_pass/reflect_stage_gate.cpp`. |
| Compile-time benchmark smoke scaffolding | Done as smoke/build evidence. | Header inclusion, 5-stage chain, 50-stage chain, aggregate `pb_compile_time_benchmarks`, and CTest labels exist. Timing budgets are not established. |
| Release/package verification scaffolding | Done for the previously validated candidate. | Package release preset, package smoke, GitHub cross-compiler workflow, release docs, evidence templates, exact code-SHA GitHub matrix evidence, and warning-clean Release smoke checks exist for SHA `87299c14c813753d170911239e251064cbbfee6f`. Cross-compiler validation has **not** been rerun on the new working-tree SHA. Publication still pending. |

### Documentation status by product surface

These are the docs that should be treated as current after the production-grade batch. If a later code change alters one of these surfaces, update the matching page in the same commit as the tests/evidence.

| Documentation area | Current truth to preserve | Pages to keep synchronized |
| --- | --- | --- |
| Branch/join and fan-in | Selected-output branch/join, explicit fan-in, and clone/projection policies (`pb::shared_view`, `pb::projected`) are supported for the documented sequential and thread-pool fan-in slices; dependency backends, preemptive cancellation, and richer error/cancellation policies are still roadmap-only. | `docs/branch-join-roadmap.md`, `docs/sequential-branch-execution-limitations.md`, `docs/fan-in-join-design.md`, this file |
| Backend support | `sequential` is the baseline backend and `thread_pool_backend` is supported only for the standard-library fan-in slice; oneTBB, Taskflow, and stdexec are not implemented. | `docs/optional-backends-roadmap.md`, `docs/research-verification-matrix.md`, `docs/roadmap-gap-map.md` |
| Error-policy DSL | Five-wrapper engine-side DSL is shipped (`throwing` / `terminating` / `ignoring` / `propagating` / `verbose`); cross-wrapper `run()` / `try_run()` parity over all topologies is still being hardened. | `README.md` "Production-grade extras" section, `include/pb/runtime/error_policy.hpp`, `docs/production-readiness.md`, `docs/roadmap-gap-map.md`, this file |
| Descriptor/export helpers | DOT / JSON / `pb::export_text` helpers cover linear, selected-output branch, and explicit fan-in pipelines; the `pb.core.graph.v1` typed contract anchors the schema-version string and byte-equal field layout, but a frozen external interchange schema remains roadmap-only. | `docs/export-schema-v1.md`, `docs/graph-export-roadmap.md`, `docs/export-helper-schema.md`, `docs/runtime-descriptor-roadmap.md` |
| C++26 feature gates | Typed `pb::features::*` `constexpr bool` constants ship and resolve to `false` on C++20; reflection adapter scaffold is gated and falls back gracefully. C++26 reflection is **not** supported on current toolchains. | `docs/cpp26-feature-gates.md`, `include/pb/core/cpp26_features.hpp`, `include/pb/adapt/reflect.hpp`, this file |
| `pb_cli` introspection | `list` / `describe` / `features` ship as built-in introspection commands with text/DOT/JSON output and `--out=` file routing for three built-in pipelines (`order-linear`, `order-branch`, `order-fan-in`); a broader stable CLI contract for arbitrary user pipelines remains future work. | `README.md` "CLI: `pb_cli`" section, `tools/pb_cli.cpp`, `tests/run_pb_cli_describe.cmake` |
| Release evidence | Exact-SHA local, CI, cross-compiler, and package evidence exists for code SHA `87299c14c813753d170911239e251064cbbfee6f`. Working tree is **189/189 local ctest**; cross-compiler validation on the new SHA is pending. | `docs/cross-compiler-validation.md`, `docs/current-release-summary.md`, `docs/production-readiness.md`, `docs/research-verification-matrix.md` |
| Remaining production gaps | Full runtime-enforced policy DSL, frozen external descriptor/export schema, optional dependency backends, benchmark budgets, modules/C++26 reflection-and-contracts integration, and release publication remain incomplete. | `docs/roadmap-gap-map.md`, this file, release notes |

## Implemented but still partial / almost production-grade

These areas work in meaningful slices, but should not be described as fully production-grade until the missing items in the right column are finished.

| Area | Implemented now | Missing before maximum / production grade |
| --- | --- | --- |
| Branch/join execution | First-match selected-output sequential branch execution, optional join stages, homogeneous outputs, variant heterogeneous outputs, type-list selected-output joins, explicit sequential fan-in joins, failed-case fan-in aggregation, void-output fan-in aggregation, borrowed move-only fan-in, stateful branch storage, observer events, move-only selected-stage consumption, thread-pool backend fan-in case scheduling, and `pb::shared_view` / `pb::projected` clone-projection policies. | oneTBB/Taskflow/stdexec backend branch execution; preemptive cancellation; backend examples/benchmarks; richer backend fan-in policy knobs; full backend branch matrix tests. |
| Heterogeneous branch outputs | `std::variant` preserves duplicate alternatives by index; type-list selected-output joins dispatch by C++ type and intentionally share overloads for duplicate same-type outputs unless the user encodes case identity into the output type. Sequential fan-in distinguishes duplicate same-type case outputs by case index in `fan_in_case_result<I, T>` and can carry failed diagnostics per case. | Tagged case-result model for selected-output joins if needed; backend/fan-in error-envelope policies; docs/examples for all future policies. |
| Move-only branch input | Predicates observe by `const input_type&`; selected branch stage may consume move-only input. Compile-fail coverage rejects predicates that consume move-only inputs by value. Sequential and thread-pool fan-in support move-only inputs when every passing case stage borrows by `const input_type&`, and `pb::shared_view<T>` lets non-copyable owned values participate in fan-in via shared-ptr semantics. By-value fan-in still requires a copy-constructible input and has targeted compile-fail coverage. | Owned per-case unique-clone policy beyond shared-view; deeper resource lifetime examples; richer observer/error edge cases. |
| Runtime descriptor/export | Runtime descriptor records and helper export fields cover current linear, selected-output branch, and explicit fan-in helper output with deterministic case/node identities and `fan_in` topology; the `pb.core.graph.v1` schema-version string is now backed by a typed contract with byte-equal goldens. | Stable versioned external schema, compatibility rules, ownership/lifetime guarantees, schema-migration policy (v1 → v2), and a documented release-grade descriptor/export API. |
| DOT / JSON / text export | Helper output is golden-tested and documented for supported linear, selected-output branch, and explicit fan-in shapes; `pb::export_text` ships alongside the JSON/DOT helpers. | Long-term schema compatibility promise, broader CLI/file export contract for arbitrary user pipelines, backend scheduling/trace export, schema migration rules, and published compatibility fixtures. |
| Runtime error model | `try_run()` captures supported failures into result-like output; expected-like propagation and runtime error formatting exist. The error-policy DSL (`throwing`/`terminating`/`ignoring`/`propagating`/`verbose` engines) is shipped as a wrapper layer with smoke tests. | Cross-wrapper `run()` / `try_run()` parity matrix, runtime-enforced `::with<pb::policy::...>` DSL beyond marker bundles, no-exception policy, serialized error records, complete `std::expected` matrix wording, and fully harmonized `run()` / `try_run()` policy docs. |
| Diagnostics | Compile-fail tests cover important current structural errors; runtime errors carry stage identity. New compile-fail diagnostics for indexed I/O alias out-of-range now name the failing alias and bound. | Stable diagnostic wording/versioning, machine-readable diagnostic schema, suggested-fix catalog, cross-compiler diagnostic parity, branch/fan-in/backend diagnostic matrix. |
| Observer/tracing | Sequential observer hooks and trace-related smoke coverage exist; the policy-DSL `verbose_engine` auto-attaches a `verbose_observer` with a stable `[pb.verbose] <event> stage=<key>` line schema. | Stable observer ABI/event schema, observer ownership/lifetime contract, tracing sink API, trace file format, cross-executor behavior, and observer/trace overhead benchmarks. |
| Stateful stage storage | Sequential stateful storage preserves linear stages and branch predicates/stages under the current policies, including non-copyable owned stage state. | Borrowed/reference, shared/unique ownership policies, reset policy, thread-local future-backend storage, external-resource cleanup policy, and public lifetime diagnostics. |
| Adapters | Free-function, member-function, function-object/functor, expected-like/result paths, current void/error behavior, and the gated `pb::reflect_stage<T>` scaffold are covered. | Working C++26 reflection-driven adapter (currently scaffold only, falls back on C++20), coroutine adapter, sender/receiver adapter, C API ownership/error adapter, runtime-bound callable adapter, full overloaded/ref-qualified member handling, reference-lifetime adapter, and policy docs. |
| Compile-time performance | Smoke targets prove representative translation units build; CMake has time-trace support. | Recorded release timing baselines, branch/join compile-time benchmark, thresholds, CI regression budget, ftime-trace aggregation, compile-time dashboard, and IWYU enforcement. |
| Cross-compiler validation | GitHub workflow covers GCC/Clang C++20/C++23, MSVC C++20, and clean Ubuntu package-release for validated code SHA `87299c14c813753d170911239e251064cbbfee6f`. | Rerun the workflow on the new working-tree SHA; MSVC C++23 if supported; Windows package-release; experimental C++26 feature-gate evidence if those gates are claimed. |

## Not implemented yet / roadmap-only

These items remain outside the supported product surface.

### 1. Fan-in / all-branches joins beyond the sequential and thread-pool slices

Sequential fan-in is shipped (`::fan_in<JoinStage>` / `::join_all<JoinStage>` aggregate all predicates, run passing cases in declaration order, accept zero passing cases, surface skipped/completed/failed slots with diagnostics, support void-output cases, and pass an ordered `pb::fan_in_results<...>` aggregate). A first thread-pool backend slice schedules passing cases concurrently while preserving declaration-order aggregate results. Fan-in clone/projection policies (`pb::shared_view`, `pb::projected`, `fan_in_uses_copy_v` / `fan_in_uses_borrow_v`) cover the owned-non-copyable input case. The full research-plan backend fan-in story is still incomplete.

Still need to implement:

```text
- preemptive cancellation policy for already-running backend case stages
- richer multi-error envelopes beyond per-case diagnostic strings
- owned per-case unique-clone policy beyond shared-view semantics
- observer events beyond the current selected/skipped/failed callbacks, such as case scheduled/completed and join started/completed
- backend scheduling/trace export semantics rather than helper graph topology only
- backend examples, compile-fail coverage for policy conflicts, runtime stress tests, docs, and benchmark coverage
```

### 2. Optional pipeline execution backends

The standard-library sequential executor is supported, and `pb::runtime::thread_pool_backend` is supported for the current fan-in backend slice. `pb::runtime::thread_pool` remains the utility underneath that backend.

Need to implement next:

```text
- backend state storage rules beyond per-run construction
- preemptive/backend cancellation policies
- richer backend observer/scheduling events
- backend examples and benchmarks
- oneTBB backend prototype
- Taskflow backend prototype
- stdexec / sender-receiver experimental backend
- dependency isolation target tests
```

### 3. Full policy DSL

Five named error-policy wrappers ship (`throwing` / `terminating` / `ignoring` / `propagating` / `verbose` engines under `pb::with_*` factories) and policy seams expose marker bundles, but the research-plan `::with<pb::policy::...>` runtime-enforced model is not complete.

Need to implement:

```text
- cross-wrapper run()/try_run() parity matrix across all five engine wrappers and every supported topology
- runtime-enforced ::with<pb::policy::...> semantics beyond marker bundles
- runtime copy/move/clone/projection policy beyond pb::shared_view + pb::projected
- runtime reference lifetime/shared-view policy
- state storage policy (borrowed/shared/unique/reset/thread-local)
- diagnostic verbosity policy
- executor capability policy
- assertion/contract policy
- policy conflict diagnostics
```

### 4. Stable external graph/export contract

The typed `pb.core.graph.v1` schema-version contract pins the literal identifier and byte-equal helper output via compile-time regression tests; `pb::export_text` ships as a compact text helper alongside JSON/DOT. This is still a typed regression net, not yet a frozen external interchange format.

Need to implement:

```text
- frozen v1 stability promise (no field removal without major bump)
- schema migration policy (v1 → v2)
- compatibility tests that fail on unintended schema changes
- broader stable descriptor-backed export API
- CLI/file export contract for arbitrary user pipelines (current pb_cli only handles built-in examples)
- examples for linear, selected-output branch, and fan-in graphs
- unsupported-shape diagnostics
```

### 5. Graph visualization CLI

`pb_cli` ships built-in `list` / `describe <name> --format=<dot|json|text> [--out=<path>]` / `features` for three built-in pipelines (`order-linear`, `order-branch`, `order-fan-in`) with CTest driver coverage. The full research-plan CLI for arbitrary user pipelines is still future work.

Need to implement:

```text
- accept user-defined pipelines (not just the built-in examples)
- documented stable CLI command surface
- filtered graph view if desired
- broader CLI tests beyond pb_cli describe driver coverage
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

The C++20 path is canonical. Typed `pb::features::*` gate constants now ship alongside their `PB_HAS_*` macro counterparts (with implication invariants and user-override support); the `pb::reflect_stage<T>` scaffold is gated and falls back to a clear `static_assert` on C++20. No reflection-powered functionality ships until a C++26 reflection toolchain baseline is established.

Need to implement later:

```text
- working reflection-driven adapter on a real C++26 reflection toolchain
- contracts integration once compiler support exists
- pack-indexing optimization paths
- richer static_assert-message integration
- broader compiler/library detection tests beyond the current fallback test
- release-grade C++26 feature-gate validation in CI
```

### 8. Release publication

Release machinery and local/package evidence exist, but a public release is not published.

Need to finish:

```text
- decide final release candidate SHA (current working tree is uncommitted past cd5a80c)
- rerun cross-compiler validation on that SHA
- update release notes with exact supported/unsupported boundaries
- tag v0.1.0
- publish GitHub release
- attach package/evidence as appropriate
```

## Production-grade priority order

Best next steps from lowest risk / highest production-readiness value to highest complexity:

```text
1. Commit the production-grade batch + 3-agent wave (uncommitted past cd5a80c) and rerun cross-compiler validation on the new SHA.
2. Harden cross-wrapper run()/try_run() parity across all five error-policy engines and every supported topology.
3. Record reproducible compile-time timing baselines for header, 5-stage, and 50-stage smoke targets.
4. Promote schema v1 from "typed regression net" toward "stable supported schema" by adding v1→v2 migration policy and stability promise docs.
5. Expand diagnostic golden coverage and suggested-fix docs for supported errors, including the new indexed I/O alias diagnostics.
6. Broaden the pb_cli surface to accept user-defined pipelines once schema migration policy is decided.
7. Finalize state/lifetime policies for owned, borrowed, shared, reset, and resource-owning stages.
8. Add owned per-case unique-clone policy on top of pb::shared_view / pb::projected for the cases shared-view semantics do not cover.
9. Add thread-pool backend examples, benchmarks, stress tests, and richer cancellation/error policy tests.
10. Add optional oneTBB / Taskflow / stdexec backends only after the thread-pool backend proves the lowering model.
11. Add modules and a real C++26 reflection adapter once toolchain baselines exist.
12. Publish v0.1.0 when release notes, exact-SHA validation, package artifact, and unsupported-boundary docs all agree.
```

## Current scorecard

```text
Linear MVP foundation:                    9.0 / 10
Sequential runtime MVP:                   8.0 / 10
Result / error current surface:           8.0 / 10  (was 7.3 — policy DSL wrappers shipped)
Adapters current surface:                 7.6 / 10  (was 7.5 — reflection scaffold gated in)
Diagnostics current surface:              7.8 / 10  (was 7.6 — indexed-I/O OOB diagnostics added)
Selected-output branch/join execution:    8.6 / 10
Fan-in join execution:                    9.0 / 10  (was 8.8 — shared_view/projected close the owned-non-copyable case)
Move-only selected-branch input support:  7.8 / 10  (was 7.6 — shared_view extends the supported envelope)
Stateful branch storage:                  8.4 / 10
DOT / JSON / text helper export:          8.8 / 10  (was 8.4 — export_text added, v1 contract pinned)
Stable descriptor/export contract:        5.5 / 10  (was 4.5 — typed v1 schema contract + byte-equal goldens)
Compile-time benchmark scaffolding:       7.2 / 10
Optional pipeline backends:               4.2 / 10
C++26 feature gates and reflection:       5.0 / 10  (new line — typed constants + gated scaffold)
Release readiness before tag:             8.2 / 10  (was 8.4 — new SHA not yet cross-compiler-validated)
Full original research-plan completion:   7.0 / 10  (was 6.4 — schema v1, policy DSL wrappers, clone/projection, C++26 gates)
```

## Main conclusion

Pipeline-c++ now has a substantial production-grade middle layer on top of the linear MVP: validated linear pipelines, a sequential runtime, adapters, result/error plumbing, a five-wrapper engine-side error-policy DSL, observer hooks (including the auto-attached verbose observer), selected-output branch/join execution, explicit sequential and thread-pool fan-in, `pb::shared_view` / `pb::projected` clone-projection policies, descriptor-backed DOT / JSON / text helper output with a typed `pb.core.graph.v1` regression contract, an expanded `pb_cli` introspection surface, typed C++26 feature gates with a fallback-tested forward-compat path, a gated reflection-adapter scaffold, indexed pipeline I/O aliases with named diagnostics, compile-time smoke targets, package smoke, and cross-compiler workflow scaffolding.

It is still not the full research-plan product. The remaining maximum-production work is concentrated in: cross-wrapper `run()` / `try_run()` policy parity, a runtime-enforced `::with<pb::policy::...>` DSL, descriptor/export *frozen* compatibility contracts, dependency pipeline backends (oneTBB / Taskflow / stdexec), broader stateful storage and ownership policies, release-grade performance budgets, C++ modules and a real C++26 reflection toolchain, and final exact-SHA release publication.
