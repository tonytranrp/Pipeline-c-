# Continue checkpoint — Pipeline-c++ long-horizon resume

Snapshot UTC: `2026-05-16T05:45:00Z`
Repo: `/mnt/c/users/tonyt/documents/github/pipeline-c++`
Current branch: `main`
Current baseline HEAD: `aa3b8f6` — `Merge commit '3b1bde497880b10735e17ef20b6ae8b2da2d3563'`
Remote status at checkpoint: `main...origin/main` with no reported ahead/behind count.

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
- Docs mostly separate supported MVP behavior from roadmap-only branch/join, graph export, runtime descriptor, optional backend, and performance-budget work.

Fresh local verification from the current baseline (refreshed during the 2026-05-16 docs-lane checkpoint):

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja --output-on-failure
```

Result: **83/83 tests passed** with the `clang-dev-ninja` preset after configure and build.

## Current working tree note

At checkpoint time the pipeline implementation was clean relative to `main`, but token/context tooling files were uncommitted:

```text
 M .gitignore
?? .contextignore
?? .github/copilot-instructions.md
?? .repomixignore
?? gen-context.config.json
```

These are RTK/SigMap/Repomix context-saving files, not pipeline feature changes. Before launching workers, decide whether the leader owns committing them, keeping them uncommitted, or excluding them from worker lanes. Do not let coding workers revert them accidentally.


## Latest team-batch checkpoint (2026-05-16)

Docs-lane monitoring observed current integration head `218664c` plus docs checkpoint `3967894` after the core/runtime/adapter/docs hardening batch:

- `371312a` / `218664c` added runtime `error_record` / `to_record(...)` diagnostic projection and smoke coverage.
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

Release-facing wording should stay narrow: these commits strengthen the current linear/sequential MVP and add branch/join marker diagnostics for unsupported boundaries, source compatibility, predicates, and invalid join-stage markers, but they do not complete branch output modeling, join compatibility validation, branch execution, graph export, stable observer ABI/event schema, optional backend execution, runtime descriptor export, or release-grade benchmark budgets. Use `docs/current-release-summary.md` and `docs/research-verification-matrix.md` as compact PR/release note seeds for this batch.

## What is done from the research plan

Treat these as shipped MVP support unless a future regression proves otherwise:

1. **Phase 1 — MVP type core:** implemented and tested through core concepts, stage traits, linear pipeline state, validation, and compile-pass/compile-fail coverage.
2. **Phase 2 — Runtime sequential execution:** implemented for validated linear pipelines with runtime tests for success, failure, exceptions, expected-like results, observer paths, and zero-stage identity.
3. **Phase 3 — User adapters:** implemented for free functions, member functions, and functors with runtime and compile-fail coverage.
4. **Phase 4 — Diagnostics/tooling foundation:** partially implemented through metadata helpers, compile-fail diagnostics, runtime error formatting, package smoke, benchmark smoke, and docs.
5. **Phase 7 — Packaging/release scaffolding:** partially implemented through install/export/package smoke checks and release-readiness docs.

## What is still roadmap-only

Do not claim these as supported until code, tests, examples, and docs land together:

- Executable branch/join topology and non-linear validation beyond marker-only unsupported-boundary/source/predicate/join-stage diagnostics.
- Graph export (`DOT`, JSON, or stable graph schema).
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

1. Audit `run()`/`try_run()` boundaries, result factories, expected-like conversion, observer lifecycle, adapter diagnostics, and runtime diagnostic record/export boundaries.
2. Add one narrow runtime hardening slice: clearer error construction, observer ordering evidence, adapter edge-case coverage, or exception-policy documentation tests.
3. Preserve current `run()` and `try_run()` semantics unless the leader scopes a policy change.
4. Keep standard-library-only design.

Avoid: async/parallel backends, dependency additions, or hidden implicit conversions.

### Worker 3 — Updater / docs / release coordination

Owns: `README.md`, `docs/*`, `examples/*`, checkpoint notes, PR/release summaries, GitHub/readiness monitoring.

Next safe queue:

1. Keep docs aligned with Worker 1/2 changes and current preset/test names, especially branch marker diagnostics and runtime descriptor/observer seams.
2. Refresh `docs/roadmap-gap-map.md`, `docs/production-readiness.md`, `docs/research-verification-matrix.md`, and release evidence notes when new behavior lands.
3. Monitor `git status`, branch divergence, and GitHub readiness. Push/pull only when explicitly authorized for the concrete action.
4. Prepare a compact PR summary with changed files, tests, risks, and remaining roadmap gaps.

Avoid: claiming roadmap-only features, editing generated build artifacts, or mixing docs-only commits with unrelated code changes.

## Leader resume checklist

Before starting the next team:

1. `git status --short --branch`
2. `git log --oneline -12`
3. Confirm what to do with token-tooling files listed above.
4. Re-run at least `cmake --preset clang-dev-ninja`, `cmake --build --preset clang-dev-ninja`, and `ctest --preset clang-dev-ninja --output-on-failure` if significant time or upstream changes passed.
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
