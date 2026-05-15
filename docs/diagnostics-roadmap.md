# Diagnostics Roadmap / Status

Diagnostics are **partially supported today**, but richer diagnostics are still a roadmap item. Treat this page as a status note that separates the current compile-fail/runtime diagnostic contract from the broader diagnostics roadmap in `research/pipeline_builder_cpp_research_plan.md`.

## Current status

Today the repository supports:

- compile-time validation failures with expected diagnostic text checked by `tests/run_compile_fail.cmake`
- intentional compile-fail coverage for linear pipeline edge/type mismatches under `tests/CMakeLists.txt`
- a public runtime diagnostic surface in `pb::runtime::error`, `error_category`, `category_name(...)`, and `describe(...)`
- a runtime diagnostic smoke test in `tests/runtime/error_diagnostic_smoke.cpp`
- a user-facing compile-fail example in `examples/error_diagnostic.cpp`
- stage names, stage records, and linear edge records that help keep error text tied to the linear pipeline stages and adjacent stage-to-stage edges that failed

Today the repository does **not** support:

- a documented guarantee that exact diagnostic wording is permanently frozen across releases
- broader structured diagnostics for graph-shaped pipelines, branch/join validation, or descriptor/export tooling
- source-location-rich diagnostic payloads, diagnostic schema/versioning, or stable machine-readable diagnostic output
- observer-driven diagnostic reporting or richer error-category policy beyond the current sequential runtime helpers
- a complete golden-test matrix that proves diagnostic behavior across every planned roadmap slice

Keep examples and release notes aligned with that boundary. The repo can truthfully claim diagnostic scaffolding and smoke coverage today, but it should not yet claim the full research-plan diagnostics story as complete.

## Why richer diagnostics matter

The research plan treats diagnostics as a product feature, not just a compiler side effect. Strong diagnostics matter because they:

- make type-level validation usable without forcing users to decode raw compiler backtraces
- turn pipeline edge mismatches into named, testable contract failures
- let runtime failures carry stage identity and error-category context instead of ad hoc strings alone
- create a stable review surface for future topology, backend, and tooling work

That direction is already visible in the current compile-fail harness and runtime error helpers, but the roadmap still aims for broader, more structured coverage than the current MVP provides.

## Intended scope relative to the research plan

The research plan describes diagnostics as an ongoing lane with several follow-on goals:

- human-readable compile-time diagnostics with targeted static assertions/concepts
- regression-friendly compile-fail tests that treat expected wording as part of the contract
- richer runtime diagnostics with stable stage/category information
- future diagnostic support for graph export, branch/join validation, observers, and optional backends
- experiments around truncation, source locations, compile-fail suite cost, and reference-stage safety

The current tree only covers the first slice of that plan. It validates the linear MVP path and provides basic runtime error descriptions, but it does not yet ship the wider diagnostic breadth implied by later research milestones.

## Non-goals for the current MVP

The current MVP should **not** claim:

- complete diagnostic coverage for every planned pipeline topology or backend
- stable machine-readable diagnostic schemas or exported diagnostic artifacts
- release-ready source-location-enriched diagnostics
- cross-compiler wording parity for every compile-fail message
- observer-integrated or graph-aware diagnostics as supported behavior

Those promises belong to later slices with explicit tests, examples, and compatibility notes.

## What must exist before richer diagnostics can be claimed

Before the repo can claim richer diagnostics support beyond the current MVP, it needs at least:

1. **Broader test coverage**  
   Compile-fail and runtime checks for the additional topology/runtime cases that the docs claim to support.
2. **User-facing examples**  
   At least one example per newly supported diagnostic class, not just internal tests.
3. **Documented compatibility boundaries**  
   Clear guidance on which wording/fields are stable and which remain experimental.
4. **Feature-aligned roadmap slices**  
   Diagnostics updated alongside branch/join, observer, runtime-descriptor, graph-export, and optional-backend work rather than described in isolation.
5. **Release-grade verification evidence**  
   Fresh passing evidence that the examples and tests prove the supported diagnostic surface on the advertised toolchains.

## Verification status today

The current verification evidence supports the existing linear-pipeline diagnostic surface:

- compile-fail tests check for expected diagnostic text through `tests/run_compile_fail.cmake`
- `pb_runtime_error_diagnostic_smoke` validates runtime category/stage/message formatting helpers
- `examples/error_diagnostic.cpp` documents the intentional compile-fail path for users
- `docs/examples.md` explains how to run the diagnostic example and the compile-fail smoke target

That evidence is enough to document the current diagnostic baseline. It is **not** enough to claim richer roadmap items such as graph-aware diagnostics, schema-stable outputs, or fully hardened wording guarantees.

## Release guidance

Until broader implementation and tests land, release notes and docs should describe diagnostics as:

> supported for the current linear MVP smoke surface, with richer diagnostics still in progress

If a future slice expands diagnostics, update this page together with:

- `docs/production-readiness.md`
- any new examples that demonstrate the added diagnostic class
- the tests that prove the added wording/shape/runtime behavior
- the roadmap pages for the feature area that introduced the new diagnostics
