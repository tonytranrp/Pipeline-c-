# Current not-done map against `research/pipeline_builder_cpp_research_plan.md`

Updated after the homogeneous sequential branch-execution hardening pass. This file is a local planning/checkpoint artifact, not a release announcement. Prefer the current tree, tests, and docs over older public-repo snapshots when deciding what is supported.

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
- Narrow linear DOT / JSON metadata-export helpers
- Compile-pass / compile-fail / runtime / example / package / benchmark smoke scaffolding
- Homogeneous sequential branch execution with optional join stages
- Stateful branch predicate/stage storage under pb::runtime::stateful_sequential
- Branch observer events for selected/skipped cases
- Branch child fallback identities for unnamed predicates/stages
- Targeted compile-fail coverage for the move-only branch-input limitation
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
- compile-time branch source/predicate/output validation
- compile-time join validation
- first-match-wins sequential branch routing
- selected branch-stage execution
- branch result/error propagation
- observer on_case_selected / on_case_skipped events
- stateful branch predicates and stages with stateful_sequential
- unnamed branch-child fallback identities such as branch.case.0.predicate
- branch runtime tests and branch examples
```

Still unsupported:

```text
- heterogeneous branch outputs joined through type_list / variant inputs
- move-only branch input execution
- multi-input join execution
- branch/join graph export
- JSON graph export for branch/join graphs
- parallel/backend branch execution
```

## Still missing compared to the research document

### 1. Heterogeneous branch / join execution

Status: **missing**

The current branch runtime supports homogeneous branch outputs. The research plan still needs heterogeneous branch outputs and richer join contracts.

Need to implement:

```text
- branch outputs represented as a type_list or variant-like join input
- join stages that can consume heterogeneous branch outputs safely
- diagnostics for unsupported / ambiguous heterogeneous joins
- runtime tests for each heterogeneous route
- examples explaining homogeneous versus heterogeneous branch support
```

### 2. Move-only branch input execution

Status: **explicitly unsupported**

`selected_branch_node` currently requires a copy-constructible `input_type` so predicates and the selected branch stage can inspect the same value. This limitation is documented and covered by `tests/compile_fail/branch_move_only_input_unsupported.cpp`.

Need to design before implementing:

```text
- borrowing/reference policy for predicates
- selected-case ownership transfer policy
- diagnostics for unsafe reuse
- tests for move-only inputs and non-copyable resources
```

### 3. Full graph export

Status: **missing**

The repo has narrow linear export helpers, but not full graph export.

Need to implement:

```text
- branch/join graph export
- stable DOT schema
- stable JSON schema
- graph export tests
- graph export examples
- docs for graph export boundaries
- export behavior for unsupported / future graph shapes
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
- branch/join descriptor records
- descriptor-backed DOT/JSON export
- descriptor examples
- descriptor-specific tests
```

### 5. Optional execution backends

Status: **missing**

The only supported backend remains the standard-library sequential runtime.

Need to implement:

```text
- thread-pool executor
- oneTBB backend
- Taskflow backend
- stdexec / sender-receiver backend
- backend feature matrix
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
5. Design heterogeneous branch-output / join semantics.
6. Design move-only branch input execution policy.
7. Define stable runtime descriptor schema.
8. Expand linear DOT helper into tested graph export boundary.
9. Add JSON graph export.
10. Create backend feature matrix.
11. Prototype thread-pool backend before oneTBB/Taskflow/stdexec.
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
Homogeneous branch/join execution:  8.2 / 10
Stateful branch storage:            8.0 / 10
Graph export:                       3.0 / 10
Stable descriptor/export contract:  3.2 / 10
Backends beyond sequential:         0.5 / 10
Release readiness:                  6.5 / 10
Full research-plan completion:      5.0 / 10
```

## Main conclusion

The repo is no longer just a basic linear MVP. It now has a real homogeneous sequential branch/join execution slice with validation, runtime routing, stateful storage, observer events, examples, and tests.

The biggest unfinished research-plan items are now:

```text
- heterogeneous branch/join execution
- move-only branch input execution
- full graph export
- JSON graph export
- stable runtime descriptor/export contract
- optional backends
- full policy DSL
- broader stateful storage/lifetime policy
- compile-time performance budgets
- cross-compiler release evidence
- v0.1.0 release publication
```
