# Current not-done map against `research/pipeline_builder_cpp_research_plan.md`

Updated after the branch/export/thread-pool bug-fix and docs alignment pass. This file is a local planning/checkpoint artifact, not a release announcement. Prefer the current tree, tests, and docs over older public-repo snapshots when deciding what is supported.

## What is now supported / mostly done

```text
Done / mostly done:
- Linear typed pipeline: pb::from<T>::then<S>::to<U>
- Stage metadata and public introspection helpers
- Compile-time linear validation
- Sequential runtime for validated linear pipelines
- Zero-stage identity pipelines
- Free-function, member-function, and function-object adapters
- Runtime result / expected-like plumbing
- Runtime observer callbacks for the sequential executor
- Linear runtime descriptor helper
- DOT / JSON metadata-export helpers for linear and supported branch pipelines
- JSON branch topology detection (`"topology":"branch"` for branch pipelines)
- Compile-pass / compile-fail / runtime / example / package / benchmark smoke scaffolding
- Homogeneous sequential branch execution with optional join stages
- First-slice heterogeneous branch outputs through `std::variant`
- Move-only selected-branch input consumption when predicates observe by `const input_type&`
- Stateful branch predicate/stage storage under pb::runtime::stateful_sequential
- Branch observer events for selected/skipped cases
- Branch child fallback identities for unnamed predicates/stages
- Standalone `pb::runtime::thread_pool` utility smoke coverage, while thread-pool pipeline backend remains roadmap
```

## Current branch/join support boundary

Sequential branch execution now has an implemented supported slice:

```cpp
using P =
  pb::from<Input>
    ::branch<
      pb::case_<PredA>::then<StageA>,
      pb::case_<PredB>::then<StageB>
    >
    ::join<JoinStage>
    ::to<Output>;
```

Supported today:

```text
- public ::branch<...> DSL
- public ::join<JoinStage> after branch
- homogeneous branch outputs
- heterogeneous branch outputs represented as `std::variant<...>`
- compile-time branch source/predicate/output validation
- compile-time join validation
- first-match-wins sequential branch routing
- selected branch-stage execution
- move-only input transfer into the selected branch stage after predicate evaluation
- branch result/error propagation
- observer on_case_selected / on_case_skipped events
- stateful branch predicates and stages with stateful_sequential
- unnamed branch-child fallback identities such as branch.case.0.predicate
- branch runtime tests, branch export tests, and branch examples
```

Still unsupported:

```text
- type_list / true multi-input join execution
- consuming predicates for move-only branch inputs
- descriptor-backed stable graph export schemas and golden outputs
- CLI/file graph export for user pipeline definitions
- parallel/backend branch execution
```

## Still missing compared to the research document

### 1. Heterogeneous branch / join execution

Status: **partial / first implementation**

The current branch runtime supports homogeneous outputs and a first heterogeneous-output slice by representing differing branch outputs as `std::variant<...>`. Join stages can consume that variant. The research plan still needs true `type_list` / multi-input join semantics and richer diagnostics.

Still needed:

```text
- type_list / true multi-input join lowering
- diagnostics for unsupported / ambiguous heterogeneous joins
- more golden/edge runtime tests for each heterogeneous route
- examples explaining homogeneous, variant-based heterogeneous, and future type-list join support
```

### 2. Move-only branch input execution

Status: **partial / first implementation**

`selected_branch_node` now supports move-only inputs when predicates are callable with `const input_type&`; runtime routing evaluates predicates through an lvalue reference and moves the input into the selected branch stage. Coverage includes `std::unique_ptr` ownership transfer into a branch stage that takes the input by value.

Still needed:

```text
- diagnostics for predicates that try to consume the move-only input before routing
- broader borrowing/reference policy docs
- unsafe-reuse diagnostics where applicable
- more tests for non-copyable resources and observer/error edge cases
```

### 3. Full graph export

Status: **partial helper surface**

The repo has DOT/JSON helpers for linear and supported branch pipelines, and JSON now reports branch topology for branch pipelines. This is not yet a descriptor-backed stable graph export contract.

Still needed:

```text
- descriptor-backed DOT/JSON export
- stable DOT schema
- stable JSON schema
- golden graph export tests
- graph export examples
- docs for schema/version boundaries
- export behavior for unsupported / future graph shapes
- CLI/file export for user pipeline definitions
```

### 4. Stable runtime descriptor / export contract

Status: **partial, not stable**

Current repo has linear descriptor helpers, but not a stable descriptor/export contract.

Need to implement:

```text
- versioned descriptor schema
- stable ownership rules
- stable lifetime rules
- stable compatibility rules
- branch/join descriptor records aligned with exporter output
- descriptor-backed DOT/JSON export
- descriptor examples
- descriptor-specific tests
```

### 5. Optional execution backends

Status: **missing**

The only supported backend remains the standard-library sequential runtime.

Need to implement:

```text
- thread-pool executor (the current `thread_pool` is a utility, not a pipeline backend)
- oneTBB backend
- Taskflow backend
- stdexec / sender-receiver backend
- backend feature matrix semantics beyond the current status table
- backend isolation from core headers
- backend tests
- backend examples
- backend benchmarks
```

### 6. Full policy DSL

Status: **partial / incomplete**

Some policy seams exist, but the full research-plan policy DSL is not complete.

Need to implement:

```text
- ::with<pb::policy::...>
- error policy DSL
- exception policy DSL
- copying / move-only policy
- reference lifetime policy
- state storage policy beyond the current sequential storage aliases
- assertion / contract policy
- diagnostics verbosity policy
- executor capability policy
```

### 7. Full stateful stage storage policy

Status: **partial**

Sequential `stateful_sequential` now preserves stored linear stages and stored branch predicates/stages. The broader public lifetime/ownership policy is still incomplete.

Need to implement and document:

```text
- borrowed reference policy
- shared ownership policy
- unique ownership policy
- explicit reset policy
- thread-local policy for future parallel backends
- state lifetime diagnostics
- state storage examples for resource-owning stages
```

### 8. Full adapter matrix

Status: **partial**

Current support is useful, but not the full research matrix.

Need to implement or harden:

```text
- stateful lambda adapter
- runtime-bound callable adapter
- void adapter
- reference-policy adapter
- coroutine adapter
- sender/receiver adapter
- C API adapter with ownership/error policy
- overloaded function handling
- ref-qualified member-function handling
- noexcept optimization path
- adapter examples for each supported shape
```

### 9. Runtime trace export

Status: **partial / early surface only**

Trace-related scaffolding exists, but not a complete trace export feature.

Need to implement:

```text
- stable trace event type
- trace sink interface
- trace serialization
- observer-to-trace adapter
- trace examples
- trace overhead benchmark
```

### 10. Stable diagnostics contract

Status: **partial**

Compile-fail and runtime diagnostic coverage is strong for current boundaries, but the stable diagnostics contract is not complete.

Need to improve:

```text
- stable diagnostic wording policy
- golden diagnostic tests for each major error
- cross-compiler diagnostic parity checks
- graph-aware diagnostics
- machine-readable diagnostic records as a stable schema
- suggested-fix diagnostics
- source-location-rich diagnostics
```

### 11. Compile-time performance baselines and budgets

Status: **scaffolding only**

Need to implement:

```text
- 5-stage compile-time baseline
- 50-stage compile-time baseline
- header inclusion cost baseline
- branch/join compile-time baseline
- CI regression budget
- ftime-trace aggregation
- compile-time dashboard
- IWYU enforcement
- threshold failure policy
```

### 12. Cross-compiler release matrix

Status: **missing evidence**

Need to collect:

```text
- GCC C++20 configure/build/test evidence
- GCC C++23 evidence
- Clang C++20 evidence
- Clang C++23 evidence
- MSVC C++20 evidence
- MSVC C++23 if supported
- experimental C++26 feature-gate evidence
- public-header compile checks across compilers
```

### 13. C++ modules

Status: **missing**

Need to implement later:

```text
- module interface unit
- module build target
- module install/export path
- module compatibility tests
- module documentation
```

### 14. C++26 feature gates

Status: **missing**

Need to implement later:

```text
- reflection adapter feature gate
- contracts integration feature gate
- pack-indexing optimization gate
- static_assert-message improvement gate
- configure-time compiler/library detection
- fallback behavior
```

### 15. Graph visualization CLI

Status: **missing**

Need to implement:

```text
- CLI prototype
- graph export command
- DOT/JSON output command
- filtered graph view
- CLI tests
- CLI docs
```

### 16. Release publication

Status: **not published**

Fresh local package-release evidence now exists for this worktree, but publication is still not done.

Collected locally in the latest hardening pass:

```text
- cmake --preset clang-dev-ninja: passed
- cmake --build --preset clang-dev-ninja: passed
- ctest --preset clang-dev-ninja --output-on-failure: passed, 124/124
- cmake --preset package-release-clang-ninja: passed
- cmake --build --preset package-release-clang-ninja: passed
- ctest --preset package-release-clang-ninja --output-on-failure: passed, 124/124
- cmake --build --preset package-release-clang-ninja --target package: passed
- package artifact: build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz
```

Still needed before publishing:

```text
- final candidate SHA decision
- release notes matching supported/unsupported boundaries
- cross-compiler evidence or explicit release limitation
- v0.1.0 tag
- published GitHub release
```

## What to improve next, in order

```text
1. Finish run() / try_run() policy hardening.
2. Add standard std::expected matrix evidence or clear fallback wording.
3. Record compile-time baselines for 5-stage and 50-stage chains.
4. Define the broader stateful stage storage/lifetime policy.
5. Finish type-list / multi-input join semantics after the current variant-based heterogeneous slice.
6. Expand move-only branch input policy beyond const-ref predicates plus selected-stage consumption.
7. Define stable runtime descriptor schema.
8. Make DOT/JSON export descriptor-backed and add golden schema tests.
9. Add CLI/file graph export only after schema tests exist.
10. Keep backend feature matrix aligned with real executor support.
11. Prototype thread-pool pipeline backend before oneTBB/Taskflow/stdexec.
12. Harden observer ABI/event ordering/lifecycle docs.
13. Add release compiler matrix evidence.
14. Decide final candidate SHA and update release notes.
15. Tag and publish v0.1.0 only after docs match test evidence.
```

## Updated status score

```text
Linear MVP foundation:              8.5 / 10
Runtime/error MVP:                  7.2 / 10
Diagnostics/validation:             7.6 / 10
Homogeneous branch/join execution:  8.5 / 10
Heterogeneous branch/join execution: 7.0 / 10
Move-only branch input support:     6.5 / 10
Stateful branch storage:            8.0 / 10
Graph export:                       5.0 / 10
Stable descriptor/export contract:  3.2 / 10
Backends beyond sequential:         0.5 / 10
Release readiness:                  6.5 / 10
Full research-plan completion:      5.0 / 10
```

## Main conclusion

The repo is no longer just a basic linear MVP. It now has a real sequential branch/join execution slice with validation, runtime routing, stateful storage, observer events, first-slice heterogeneous outputs through `std::variant`, move-only selected-branch input consumption, examples, and tests.

The biggest unfinished research-plan items are now:

```text
- type-list / multi-input join execution
- broader move-only branch input policies and diagnostics
- descriptor-backed stable graph export
- CLI/file graph export
- stable runtime descriptor/export contract
- optional pipeline backends
- full policy DSL
- broader stateful storage/lifetime policy
- compile-time performance budgets
- cross-compiler release evidence
- v0.1.0 release publication
```
