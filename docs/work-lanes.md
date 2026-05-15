# Coordinated Work Lanes

This project is being hardened through four reviewable production-readiness lanes. Each lane owns a distinct surface so changes stay small, testable, and easy to merge.

## Lane 1: Architecture, API, and core DSL

- Owns public pipeline APIs under `include/pb/`.
- Owns typed stage metadata, compile-time validation, diagnostics, and header hygiene.
- Success evidence: compile-pass and compile-fail tests that prove DSL acceptance and rejection paths.

## Lane 2: Runtime, executor, and error model

- Owns sequential execution behavior, `result`/error propagation, runtime diagnostics, and executor abstractions.
- Keeps safe failure behavior explicit before optional parallel or async backends are added.
- Success evidence: runtime smoke tests for value propagation, expected-like failures, and zero-stage behavior.

## Lane 3: CMake, packaging, and tooling

- Owns targets, install/export config, package-consumer behavior, presets, warning options, optional tooling, and benchmark toggles.
- Keeps build hygiene independent from API/runtime feature work.
- Success evidence: configure/build/install/package-consumer smoke checks and preset coverage.

## Lane 4: Tests, CI, and regressions

- Owns compile-pass, compile-fail, runtime, package-consumer, example, and regression test coverage.
- Converts bug fixes and readiness gaps into repeatable checks before behavior changes land.
- Success evidence: focused regression tests plus full local `ctest` runs.

## Coordination rules

- Prefer one lane-owned change per commit.
- Touch shared files only when the lane's success evidence requires it.
- Include the verification command and result in the task report so the leader can reconcile worktrees before shutdown.
