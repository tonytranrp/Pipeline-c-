# Branch / Join Roadmap / Status

Branch and join support is **not** a supported feature in the current tree. Treat this page as a roadmap/status note for planned non-linear pipeline composition, not as public API documentation.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time metadata/introspection through `describe()` and stage records
- sequential runtime execution for validated linear pipelines
- branch marker aliases (`case_`, `branch_case`, `branch_node`, `join_node`) that lock the current unsupported boundary, branch source compatibility, predicate marker, and invalid join-stage requirements from research lines 468-479, 583-591, and 989-997

Today the repository does **not** support:

- public `branch`, `fork`, `select`, or `join` pipeline builders
- multi-output lowering into labeled branch paths
- branch output modeling, multi-input join compatibility validation, or execution
- supported branch/join examples, benchmarks, or executor coverage

Keep release notes and examples aligned with that boundary. The marker/diagnostic slice is evidence for the unsupported boundary only; do not present branch/join execution or graph topology as shipped just because the research plan already sketches the feature.

## Why branch/join matters

Branch/join support is the main step from a linear pipeline DSL to a richer graph model. The research plan treats it as the feature that enables:

- parallel or conditional flow through multiple stage paths
- explicit join points after divergent processing
- richer graph export and execution backends
- diagnostics for multi-input and multi-output pipeline shapes

Without this feature, the current library remains intentionally limited to one validated chain of stages.

## Intended scope relative to the research plan

The research plan treats branch/join as planned work, not current behavior:

- branch and join modeling is explicitly deferred until after the MVP linear chain
- a future branch graph builder slice introduces `fork`, `select`, `join`, and branch labels
- future validation work covers branch predicate checks, join compatibility, and branch diagnostics
- future runtime work covers sequential branch execution before more advanced backends

The current repository documents the intent, but it does not yet ship the public API, validation rules, or execution model needed to claim support.

## Non-goals for the current MVP

The current MVP should **not** claim:

- multi-input stage support before join support exists
- multi-output branch lowering for ordinary stage outputs
- feedback/cycle execution
- stable branch label syntax or join result policy

Those decisions belong to a later implementation slice with explicit tests, examples, and executor contracts.

## What would be required before claiming support

Before branch/join can move from roadmap to supported behavior, the repo needs:

1. **A defined public graph-builder API**  
   Stable type names, label semantics, topology rules, and composition syntax.
2. **Compile-time validation coverage**  
   Checks for branch predicates, label mapping, multi-output routing, and join compatibility.
3. **Runtime execution coverage**  
   At minimum, one supported executor path that proves branch selection and join behavior work correctly.
4. **Targeted tests and examples**  
   Positive tests, negative diagnostics, and at least one user-facing example that reflects the supported shape.
5. **Follow-on documentation and performance evidence**  
   Updated docs, graph-export alignment, and evidence about validation/execution cost once the feature exists.

## Verification status today

The current verification evidence covers the existing linear pipeline core plus marker-only branch/join boundary diagnostics, not executable branch/join behavior:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

Current branch/join-specific verification is limited to compile-pass public marker aliases and compile-fail diagnostics for unsupported topology, branch source compatibility, predicate marker misuse, and invalid join-stage markers. There are still no supported branch/join examples, benchmarks, branch output modeling tests, join compatibility tests, or executor tests.

## Release guidance

Until implementation and tests land, release notes and docs should describe branch/join support as:

> marker-only unsupported-boundary, branch source/predicate, and invalid join-stage diagnostics exist; executable branch/join topology remains roadmap work

If a future slice adds branch/join support, update this page together with:

- `docs/production-readiness.md`
- graph-export and observer/tracing docs when those surfaces begin to depend on graph topology
- the relevant tests/examples/bench entries that validate topology, diagnostics, and runtime behavior
