Checked current `main` again. Current public repo has **384 commits**, still shows **no published releases**, and README still frames it as a **compile-time validated C++20 pipeline-builder foundation**, not a finished full roadmap implementation. ([GitHub][1])

## What changed / what you now have

You now have more than the older MVP list:

```text
Done / mostly done:
- Linear typed pipeline: pb::from<T>::then<S>::to<U>
- Stage metadata
- Compile-time linear validation
- Sequential runtime for validated linear pipelines
- Zero-stage identity pipelines
- Free-function and function-object adapters
- Public introspection helpers
- Compile-pass / compile-fail / runtime / package / benchmark smoke scaffolding
```

The production-readiness doc explicitly lists those as supported today. 

```text
Partial / validation-only:
- Branch marker aliases
- branch_case_output / branch_outputs metadata
- branch source compatibility checks
- branch predicate marker validation
- homogeneous branch-node input validation
- branch-output compatibility validation
- join-consumption validation
- invalid join-stage diagnostics
- branch-output marker misuse diagnostics
- runtime diagnostic projection: error_record / to_record(...)
- observer callbacks for sequential runtime
- linear descriptor helper
- narrow linear DOT helper
```

The roadmap gap map says branch/join is currently **validation support only**, with non-linear execution and branch routing still unimplemented.  The production-readiness doc also says the linear DOT/export helper is only narrow linear evidence, not full graph export. 

## Still missing compared to the research document

### 1. Sequential branch execution

Status: **partially done**

The branch runtime path is now implemented and tested, but the full public branch/join topology and execution contract is incomplete.

Current progress vs earlier plan:

- Implemented: branch runtime routing, selected-case evaluation, branch predicate execution, branch result/error propagation, runtime branch tests
- Remaining: verified public `::branch<...>::join<...>` DSL/descriptor/join-execution path and richer branch-execution examples

```text
- observer callbacks for branch paths (runtime coverage now present)
- branch execution tests (runtime coverage present)
- branch execution examples (minimal smoke coverage present; more user-facing examples still needed)
```

The research document lists sequential branch execution as part of Phase 5.

### 2. Real public branch/join DSL

Status: **missing / marker-only**

Need to implement user-facing syntax close to:

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

Right now the repo has marker aliases and validation helpers, but the docs say executable branch/join topology remains roadmap-only.  The research doc explicitly includes `::branch<...>::join<...>` as the target DSL. 

### 3. Full graph export

Status: **missing**

You have a narrow linear DOT helper, but not full graph export.

Need to implement:

```text
- branch/join graph export
- JSON export
- stable DOT schema
- stable JSON schema
- graph export tests
- graph export examples
- graph export docs
- graph export runtime failure/error handling
```

The research plan calls for `pb::export_dot(...)` and `pb::export_json(...)`.  Current repo docs state that the current DOT helper is only linear and full graph export remains roadmap-only. 

### 4. Stable runtime descriptor / export contract

Status: **partial, not stable**

Current repo has a linear descriptor helper, but not a stable descriptor/export contract.

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

The roadmap gap map says `pb::make_descriptor<Pipeline>()` and `engine.descriptor()` exist as narrow linear helpers, but stable export/compatibility is still roadmap-only. 

### 5. Optional execution backends

Status: **missing**

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

The research plan lists thread-pool, oneTBB, Taskflow, stdexec, and backend feature matrix as Phase 6.  Current repo docs say the only supported backend remains the standard-library sequential runtime. 

### 6. Full policy DSL

Status: **missing**

Need to implement:

```text
- ::with<pb::policy::...>
- error policy DSL
- exception policy DSL
- copying / move-only policy
- reference lifetime policy
- state storage policy
- assertion / contract policy
- diagnostics verbosity policy
- executor capability policy
```

The research document defines policies as type parameters, with examples like `::with<pb::policy::errors::expected>`.  Current repo docs still describe broader exception-policy DSL as future work. 

### 7. Full stateful stage storage policy

Status: **partial / not public guarantee**

Need to implement and document:

```text
- default-construct-per-run policy
- store-in-engine policy as stable API
- borrowed reference policy
- shared ownership policy
- unique ownership policy
- move-only state policy
- reset policy
- thread-local policy for future parallel backends
- state lifetime diagnostics
```

The research plan requires stateful stage storage in Phase 2 and explicit state/lifetime handling in the integration plan.  The current roadmap says explicit stateful stage storage policy is not a current public guarantee. 

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

The research plan lists free functions, function pointers, stateless/stateful function objects, lambdas, member functions, coroutines, senders, exception-throwing functions, void-returning functions, references, move-only values, and C API wrappers as target user-code shapes. 

### 9. Runtime trace export

Status: **missing**

Need to implement:

```text
- trace event type
- trace sink interface
- trace serialization
- runtime trace export
- observer-to-trace adapter
- trace examples
- trace overhead benchmark
```

The research plan includes runtime trace export under Phase 4 diagnostics/tooling.  Current observer support is only partial and sequential-runtime scoped. 

### 10. Stable diagnostics contract

Status: **partial**

Need to improve:

```text
- stable diagnostic wording
- golden diagnostic tests for each major error
- cross-compiler diagnostic parity checks
- graph-aware diagnostics
- machine-readable diagnostic records
- suggested-fix diagnostics
- source-location-rich diagnostics
```

Current docs say public diagnostics are covered by compile-fail smoke tests, but exact wording is still being hardened.  The research plan treats readable diagnostics and compile-fail tests as product features. 

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

The research document requires compile-time baselines, regression tracking, and compile-time benchmarks as release-quality gates.  Current repo docs say benchmark scaffolding exists, but release thresholds and CI regression budgets are not established. 

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

The research plan requires compiler matrix coverage and public C++20/C++23 support boundaries.  Current repo docs say cross-compiler validation beyond the local configured toolchain remains a release gate. 

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

The research document recommends modules later to reduce include cost, while keeping headers available. 

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

The research plan says C++26 reflection, contracts, pack indexing, and better static assertions should stay feature-gated until the compiler matrix proves support. 

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

The research checklist includes CLI prototype work under tooling. 

### 16. Release publication

Status: **missing**

Need to implement / collect:

```text
- fresh package-release-clang-ninja configure evidence
- fresh build evidence
- fresh CTest evidence
- fresh package target evidence
- package artifact path
- release notes
- v0.1.0 tag
- published GitHub release
```

The research plan ends Phase 7 with package config, docs, CI compiler matrix, and publishing `v0.1.0`.  GitHub currently shows no releases published. ([GitHub][1])

## What to improve next, in order

```text
1. Finish run() / try_run() policy hardening.
2. Add standard std::expected matrix evidence or clear fallback wording.
3. Record compile-time baselines for 5-stage and 50-stage chains.
4. Define explicit stateful stage storage policy.
5. Convert branch/join validation into sequential branch execution.
6. Add branch execution runtime tests.
7. Add branch execution examples.
8. Define stable runtime descriptor schema.
9. Expand linear DOT helper into tested graph export boundary.
10. Add JSON export.
11. Create backend feature matrix.
12. Prototype thread-pool backend before oneTBB/Taskflow/stdexec.
13. Harden observer ABI/event ordering/lifecycle docs.
14. Add release compiler matrix evidence.
15. Run package-release evidence on exact candidate SHA.
16. Tag and publish v0.1.0 only after docs match test evidence.
```

## Updated status score

```text
Linear MVP foundation:              8.2 / 10
Runtime/error MVP:                  7.0 / 10
Diagnostics/validation:             7.2 / 10
Branch/join validation:             6.5 / 10
Executable branch/join:             1.5 / 10
Graph export:                       2.5 / 10
Stable descriptor/export contract:  3.0 / 10
Backends beyond sequential:         0.5 / 10
Release readiness:                  5.5 / 10
Full research-plan completion:      4.4 / 10
```

## Main conclusion

The repo is no longer just a basic linear MVP. It now has meaningful validation and diagnostic infrastructure around branch/join boundaries, runtime diagnostic identity, observer checks, a linear descriptor helper, and a narrow DOT helper.

The biggest unfinished research-plan items are still:

```text
- sequential branch execution
- real public branch/join DSL execution
- full graph export
- JSON export
- stable runtime descriptor/export contract
- optional backends
- full policy DSL
- stateful stage storage policy
- compile-time performance budgets
- cross-compiler release evidence
- v0.1.0 release publication
```

[1]: https://github.com/tonytranrp/Pipeline-c- "GitHub - tonytranrp/Pipeline-c-: Pipeline-builder · GitHub"
