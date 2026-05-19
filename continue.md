# Continue checkpoint — Pipeline-c++ long-horizon resume

Snapshot UTC: `2026-05-18T20:40:00Z`
Repo: `/Users/tonytran/Documents/GitHub/Pipeline-c-`
Current branch: `main`
Latest code-validation HEAD: `6805543ede6946aa283be7f24fb3736c762f47b2` — cross-compiler validation passed
Remote status at checkpoint: pushed to `origin/main` at the validation SHA before this docs-only refresh.

This checkpoint replaces older OMX team pause notes. Use it to resume the next 3-agent long-horizon wave after usage resets without re-reading the entire prior team transcript.

## Current verified product state

The current tree is a **strong MVP foundation** for `research/pipeline_builder_cpp_research_plan.md`:

- C++20 target-based CMake project with `pb::core`, `pb::runtime`, and `pb::pipeline`.
- Typed linear DSL: `pb::from<T>::then<S>::to<U>`.
- Compile-time validation for stage shape, adjacent edge type compatibility, and final sink compatibility.
- Stage traits, stage names/keys, optional error metadata, descriptor helpers, and linear pipeline introspection.
- Free-function, member-function, and functor adapters for existing user code.
- Sequential runtime with `run()`, `try_run()`, result/expected-like plumbing, zero-stage identity behavior, and observer hooks.
- Compile-pass, compile-fail, runtime, example, package-consumer, and benchmark smoke scaffolding.
- Sequential branch/join support for the current standard-library sequential runtime: homogeneous outputs, variant-based heterogeneous outputs plus type-list selected-output joins including duplicate alternatives by index, optional joins, observer case events, stateful branch predicate/stage storage, and move-only selected-stage consumption when predicates inspect by `const input_type&`.
- DOT/JSON descriptor-backed helper export for linear and supported branch pipelines, including JSON branch topology, DOT label escaping, and helper-output golden regressions.
- Docs separate supported branch/export helper behavior from roadmap-only parallel fan-in joins, stable descriptor/export schema guarantees, CLI/file export, optional backends, runtime descriptor stability, and performance-budget work.

Fresh cross-compiler validation from the current code snapshot:

```text
validated_code_sha=6805543ede6946aa283be7f24fb3736c762f47b2
workflow=https://github.com/tonytranrp/Pipeline-c-/actions/runs/26058848575
GCC C++20: passed, 150/150
GCC C++23: passed, 150/150
Clang C++20: passed, 150/150
Clang C++23: passed, 150/150
MSVC C++20: passed, 149/149
package-release clean Ubuntu: passed, 150/150 plus TGZ package generated
```

## Current working tree note

At the validation checkpoint, `main` was pushed and GitHub Actions passed on SHA `6805543`. This docs-only refresh may create a later SHA; rerun the matrix before release tagging if exact tag-SHA evidence is required.


## Latest team-batch checkpoint (updated 2026-05-18)

Docs-lane monitoring observed current integration head `30447ae` plus runtime diagnostic identity integration `c13a54c` / `2939908` and docs checkpoint `1182b6d` after the core/runtime/adapter/docs hardening batch:

- `4be3ab1` / `3e48488` locked linear descriptor, observer failure callback, and `try_run()` error identity consistency.
- `371312a` / `218664c` added runtime `error_record` / `to_record(...)` diagnostic projection and smoke coverage.
- `c13a54c` / `2939908` preserved/filled custom expected-like diagnostic stage identity and aligned `error_record` / observer failure/exception stage identity with `engine.describe()` in the linear runtime path.
- `30447ae` required homogeneous `branch_node` case inputs and locked the mismatch diagnostic while staying marker-only.
- `4c6419e` added marker-only `branch_case_output` / `branch_outputs` metadata scaffolding and public-header coverage.
- `5afcb3a` / `f0652be` added compile-fail diagnostics for branch-output marker misuse.
- `dbb8d5b` added marker-only `join_node` validation and invalid join-stage diagnostics.
- `b737175` checked branch_case source compatibility at the marker boundary.
- `b293a65` added sequential observer accessor coverage in the runtime path.
- `8a6ea8a` validated branch predicate marker shape while keeping branch execution unsupported.
- `4722296` added branch marker aliases and an explicit unsupported-topology compile-fail boundary.
- `8b5fda6` strengthened descriptor alias symmetry for the current linear introspection helpers.
- `7a483b7` added adapter/member hardening in `include/pb/adapt/fn.hpp`.
- `ee412f7` added public-header coverage for core `stage_traits` aliases and tightened related diagnostic misuse cases.
- `3b1a231` / `23f1d60` hardened runtime `error_or(...)` fallback selection for expected-like/result normalization boundaries.
- `ec45eae` / `62820ed` / merge `caa43ee` carried sequential observer replacement/accessor coverage into the integrated history.

Release-facing wording should stay narrow but current: branch/join execution is now supported for the sequential slice, including homogeneous outputs, variant-based heterogeneous outputs plus type-list selected-output joins with duplicate alternatives by index, move-only selected-stage input consumption, observer events, stateful branch storage, and compile-time join validation. DOT/JSON export is helper-level for linear and supported branch pipelines. Do not claim parallel fan-in joins, stable descriptor/export compatibility, CLI/file export, optional backend execution, stable observer ABI/event schema, runtime descriptor export, or release-grade benchmark budgets. Use `docs/current-release-summary.md`, `docs/cross-compiler-validation.md`, and `docs/research-verification-matrix.md` as compact PR/release note seeds.

## What is done from the research plan

Treat these as shipped MVP support unless a future regression proves otherwise:

1. **Phase 1 — MVP type core:** implemented and tested through core concepts, stage traits, linear pipeline state, validation, and compile-pass/compile-fail coverage.
2. **Phase 2 — Runtime sequential execution:** implemented for validated linear pipelines with runtime tests for success, failure, exceptions, expected-like results, observer paths, and zero-stage identity.
3. **Phase 3 — User adapters:** implemented for free functions, member functions, and functors with runtime and compile-fail coverage.
4. **Phase 4 — Diagnostics/tooling foundation:** partially implemented through metadata helpers, compile-fail diagnostics, runtime error formatting, package smoke, benchmark smoke, and docs.
5. **Phase 7 — Packaging/release scaffolding:** partially implemented through install/export/package smoke checks, cross-compiler validation workflow/evidence, and release-readiness docs.

## What is still roadmap-only

Do not claim these as supported until code, tests, examples, and docs land together:

- Parallel all-branches fan-in / true backend multi-input join execution beyond the supported selected-output sequential branch slice.
- Stable descriptor/export schema, CLI/file export, and backend graph-export semantics beyond helper DOT/JSON output.
- Stable runtime descriptor/export contract.
- Optional Taskflow/oneTBB/stdexec backends.
- Fully unified exception/error policy between `run()` and `try_run()`.
- Release-grade compile-time/performance thresholds and CI-enforced budgets.
- Stable, frozen diagnostic wording/schema across all future slices.

## Recommended next 3-agent wave

Use the 3-agent structure from `docs/work-lanes.md`.

### Worker 1 — Core API / diagnostics coding

Owns: `include/pb/core/*`, `include/pb/pipeline.hpp`, compile-pass/compile-fail tests.

Next safe queue:

1. Audit `stage_traits`, `describe`, public aliases, and validation diagnostics against the research plan.
2. Add one narrow compile-time ergonomics improvement only if it has an obvious test target.
3. Prefer diagnostic clarity and alias symmetry over new topology features.
4. Add compile-pass and compile-fail coverage for every public API behavior change.

Avoid: branch/join, graph export, expression-template rewrites, or broad public API churn.

### Worker 2 — Runtime/adapters/diagnostics coding

Owns: `include/pb/runtime/*`, `src/runtime/*`, `include/pb/adapt/*`, runtime/adapter tests.

Next safe queue:

1. Audit `run()`/`try_run()` boundaries, result factories, expected-like conversion, observer lifecycle, adapter diagnostics, and runtime diagnostic record/export boundaries, and descriptor/observer/error identity consistency.
2. Add one narrow runtime hardening slice: clearer error construction, observer ordering evidence, adapter edge-case coverage, or exception-policy documentation tests.
3. Preserve current `run()` and `try_run()` semantics unless the leader scopes a policy change.
4. Keep standard-library-only design.

Avoid: async/parallel backends, dependency additions, or hidden implicit conversions.

### Worker 3 — Updater / docs / release coordination

Owns: `README.md`, `docs/*`, `examples/*`, checkpoint notes, PR/release summaries, GitHub/readiness monitoring.

Next safe queue:

1. Keep docs aligned with Worker 1/2 changes and current preset/test names, especially branch marker/output diagnostics and runtime descriptor/observer seams.
2. Refresh `docs/roadmap-gap-map.md`, `docs/production-readiness.md`, `docs/research-verification-matrix.md`, and release evidence notes when new behavior lands.
3. Monitor `git status`, branch divergence, and GitHub readiness. Push/pull only when explicitly authorized for the concrete action.
4. Prepare a compact PR summary with changed files, tests, risks, and remaining roadmap gaps.

Avoid: claiming roadmap-only features, editing generated build artifacts, or mixing docs-only commits with unrelated code changes.

## Leader resume checklist

Before starting the next team:

1. `git status --short --branch`
2. `git log --oneline -12`
3. Confirm what to do with token-tooling files listed above.
4. Re-run at least `cmake --preset clang-dev-ninja`, `cmake --build --preset clang-dev-ninja`, and `ctest --preset clang-dev-ninja --output-on-failure` if significant time or upstream changes passed; rerun `cross-compiler-validation.yml` before release tagging if non-doc code changed after `6805543`.
5. Launch the 3-agent wave with finite batch tasks and ask each worker to report changed files, tests, risks, and next task.

Before ending the next team wave:

1. Integrate or explicitly defer each worker output.
2. Run the developer preset configure/build/CTest.
3. Run package-release configure/build/CTest/package if packaging/docs/release surfaces changed.
4. Rewrite or replace any system-generated auto-checkpoint commits with Lore-format commits before final merge if history is still local and safe to rewrite.
5. Update this file with the new HEAD, test count, known risks, and next safe queue.

## Token-saving note

Use the local token-saving workflow during resume:

```bash
rtk tokrun -- cmake --build --preset clang-dev-ninja
rtk tokrun -- ctest --preset clang-dev-ninja --output-on-failure
tokgain
```

If `tokrun` reports failures, warnings, or missing detail, inspect the referenced `raw_log=...` file or rerun with `tokrun --raw` before making code decisions.
