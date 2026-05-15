# Observer Hooks Roadmap / Status

Observer hooks are **not** a supported feature in the current tree. Treat this page as a roadmap/status note for planned runtime observation and tracing hooks, not as public API documentation.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time metadata/introspection through `describe()` and stage records
- sequential runtime execution with the current result/error plumbing

Today the repository does **not** support:

- a public observer interface or callback registration API
- runtime `set_observer(...)` hooks on the shipped execution surface
- structured start/finish/error/value tracing events
- observer-hook examples, tests, or benchmark coverage

Keep release notes and examples aligned with that boundary. Do not present observer hooks as partially shipped just because the research plan already sketches the feature.

## Why observer hooks matter

Observer hooks are part of the project’s operational story, not just a convenience feature. The research plan treats them as a way to make runtime execution easier to inspect, debug, and measure without forcing users to infer behavior from compiler backtraces or ad hoc logging.

For this project, observer hooks would eventually help with:

- stage-level execution visibility
- structured error/event reporting
- optional trace export and runtime diagnostics
- measuring feature cost when tracing is disabled, compiled out, or enabled

## Intended scope relative to the research plan

The research plan treats observer hooks as planned work, not current behavior:

- it explicitly calls for observer hooks alongside graph export and test harness improvements
- it sketches a future runtime surface that includes `set_observer(pb::observer*)`
- it lists runtime tests for observer hooks as future verification work
- it tracks follow-on trace/export work and cost measurement as separate slices

In other words, the current repo documents the intent, but it does not yet ship the API or validation needed to claim support.

## Non-goals for the current MVP

The current MVP should **not** claim:

- stable observer ABI or ownership rules
- built-in logging/tracing sinks
- guaranteed event schemas or trace file formats
- backend-agnostic observer behavior across future executors

Those decisions belong to a later implementation slice with tests, examples, and explicit runtime contracts.

## What would be required before claiming support

Before observer hooks can move from roadmap to supported behavior, the repo needs:

1. **A defined public API surface**  
   Stable type names, lifetime/ownership rules, event semantics, and enable/disable behavior.
2. **Runtime implementation coverage**  
   Sequential executor integration that proves hooks actually observe stage start, completion, and failure paths.
3. **Targeted tests**  
   Positive runtime tests plus boundary/negative tests for invalid sinks, disabled tracing paths, or unsupported event flows.
4. **Documentation and examples**  
   One supported usage path and one clearly bounded failure/limitation example.
5. **Cost validation**  
   Evidence for tracing-disabled vs tracing-enabled overhead before treating hooks as production-ready.

## Verification status today

The current verification evidence covers the existing pipeline core, not observer hooks:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

There is currently no observer-hook-specific implementation or verification target under `tests/`, `examples/`, or `bench/`.

## Release guidance

Until implementation and tests land, release notes and docs should describe observer hooks as:

> roadmap work only; unimplemented and unverified in the current tree

If a future slice adds observer hooks, update this page together with:

- `docs/production-readiness.md`
- any runtime API docs or examples that expose the hook surface
- the relevant tests/bench entries that validate event behavior and cost
