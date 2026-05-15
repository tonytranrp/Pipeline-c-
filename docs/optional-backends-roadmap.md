# Optional Backends Roadmap / Status

Optional backend execution is **not** a supported feature in the current tree. Treat this page as a roadmap/status note for planned non-core runtime backends, not as public API documentation.

## Current status

Today the repository supports:

- a standard-library-only core/runtime surface for the shipped MVP
- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- sequential runtime execution for validated linear pipelines
- build-time feature flags that reserve space for future backend experiments

Today the repository does **not** support:

- a shipped thread-pool executor backend
- public oneTBB, Taskflow, or stdexec backend adapters
- backend-specific examples, tests, or benchmarks proving execution behavior
- stable async/parallel execution semantics across multiple executors

Keep release notes and examples aligned with that boundary. Do not present optional backends as partially shipped just because CMake options or roadmap notes already exist.

## Why optional backends matter

Optional backends are the path from the current deterministic sequential executor to richer execution models. The research plan treats them as important because they can eventually provide:

- parallel execution for suitable pipeline shapes
- integration with established task-graph and filter-pipeline runtimes
- experimental sender/receiver execution models once compiler support is practical
- backend-specific tuning for token limits, scheduling, and concurrency policy

At the same time, the research plan is explicit that these backends should remain isolated from the core DSL and only arrive after the basic validation/runtime story is stable.

## Intended scope relative to the research plan

The research plan treats optional backends as planned work, not current behavior:

- sequential execution is the first supported backend
- a simple internal thread-pool backend comes next
- oneTBB and Taskflow remain optional adapter targets
- stdexec/sender-receiver support stays experimental until compiler/library support stabilizes
- backend-specific headers and dependencies must stay out of public core headers

The current repository documents the direction, but it does not yet ship the runtime adapters, stable API boundaries, or verification needed to claim backend support beyond sequential execution.

## Non-goals for the current MVP

The current MVP should **not** claim:

- production-ready parallel execution semantics
- stable external dependency integration for oneTBB, Taskflow, or stdexec
- backend-agnostic cancellation, scheduling, or token-limit policy
- public backend headers in the default user include surface

Those decisions belong to later slices with explicit adapter targets, tests, and performance evidence.

## What would be required before claiming support

Before optional backends can move from roadmap to supported behavior, the repo needs:

1. **A defined backend adapter surface**  
   Stable target names, feature-gate behavior, dependency rules, and error/ownership expectations.
2. **Implementation coverage for each backend**  
   A real adapter path for the chosen backend, not just a CMake option placeholder.
3. **Targeted tests and examples**  
   Positive runtime tests, negative/configuration-failure checks, and at least one backend-specific example where support is claimed.
4. **Isolation proof**  
   Evidence that backend dependencies stay out of the core public headers and default build surface.
5. **Performance and behavior evidence**  
   Validation for concurrency/tokens/scheduling behavior plus compile-time/runtime cost before treating the backend as production-ready.

## Verification status today

The current verification evidence covers the existing sequential pipeline core, not optional backend execution:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

There is currently no optional-backend-specific implementation or verification target under `tests/`, `examples/`, or `bench/`.

## Release guidance

Until implementation and tests land, release notes and docs should describe optional backends as:

> roadmap work only; unimplemented and unverified in the current tree

If a future slice adds optional backends, update this page together with:

- `docs/production-readiness.md`
- any backend-specific examples or API docs
- build guidance for enabling and verifying those backends
- the relevant tests/bench entries that validate isolation, runtime behavior, and cost
