# Sequential Branch Execution — Supported Features and Limitations

Sequential branch routing with optional join stages is now supported for homogeneous outputs, first-slice heterogeneous outputs through `std::variant`, selected-output type-list joins, and move-only selected-branch input consumption. This page documents what works today and what remains roadmap work.

## Supported

- **Public DSL**: `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>`
- **Compile-time validation**: join context enforced (must follow `::branch<...>`), type compatibility checked, branch predicates validated, and branch outputs mapped to either a single homogeneous type or `std::variant<...>` when they differ
- **Runtime execution**: first-match-wins predicate evaluation with short-circuit, observer events (`on_case_selected`, `on_case_skipped`), error propagation with `[branch]` prefix annotation
- **Stateful storage**: branch predicates and stages respect `pb::runtime::stateful_sequential`, preserving mutable state across multiple pipeline runs
- **Join stages**: `::join<JoinStage>` appends a linear stage after the branch node, validated for type compatibility, including join stages that consume the heterogeneous branch `std::variant` or declare the raw branch output `pb::meta::type_list<...>` and overload every output type
- **Observer coverage**: standard observer events (`on_stage_start`, `on_stage_success`, `on_stage_failure`, `on_stage_exception`) plus branch-specific events (`on_case_selected`, `on_case_skipped`)
- **Multiple cases**: any number of branch cases (1+) supported via recursive template evaluation
- **Move-only selected input**: move-only branch inputs can be routed when predicates accept `const input_type&`; the selected branch stage receives the moved value

## Not yet supported

### Parallel fan-in / all-branch joins
Branch cases with different output types currently produce a selected branch result: either the single homogeneous output type or a `std::variant<...>` for heterogeneous cases. A join may declare `pb::meta::type_list<...>` to select overloads for that one routed output, but true all-branches fan-in that runs multiple branch paths and passes separate typed values to a join is not implemented.

Current type model summary:

- branch outputs are tracked as a raw `pb::meta::type_list<...>` of case output types
- when case outputs differ, the execution boundary uses a unified `std::variant<...>` for the selected branch result
- a branch case or join stage may still use `pb::runtime::result<T>` for value-or-error execution, but that is orthogonal to the branch type model
- the type-list join path is selected-output dispatch: the join stage declares `pb::meta::type_list<...>` and overloads each raw output type
- there is no per-case tagged result wrapper or tuple-style fan-in type for the current sequential branch path
- the branch type model does not use `std::optional<...>` per case; the branch either selects one output value or reports a runtime contract error if no case matches

### Consuming predicates for move-only branch inputs
Move-only branch inputs are supported only when predicates can inspect the input by `const input_type&`. A predicate that must consume the move-only input before routing is not supported, because the selected branch stage still needs to receive the value.

### Branch-specific observer event ordering
The observer interface provides `on_case_selected` and `on_case_skipped` events, but the exact ordering relative to other observer events is documented as the current implementation behavior, not a stable contract.

Current observer sequence for a branch run is:

- `on_stage_start(branch)` when the branch node starts
- `on_stage_start(predicate)` / `on_stage_success(predicate)` for each evaluated predicate
- `on_case_skipped(branch, index, predicate)` for a false predicate
- `on_case_selected(branch, index, predicate)` for the first true predicate
- `on_stage_start(case_stage)` / `on_stage_success(case_stage)` for the selected case stage
- `on_stage_start(join_stage)` / `on_stage_success(join_stage)` for a following join stage, when present

`on_stage_failure(...)` and `on_stage_exception(...)` are used when the predicate, case stage, or join stage fails or throws; the branch-specific callbacks do not replace the standard failure callbacks.

### Unnamed stage identity collisions
When predicates and branch stages do not define `stage_key()` / `stage_name()`, the runtime generates fallback identities from the branch node's `StageIndex`. Multiple unnamed predicates or stages may share the same numeric fallback, making them harder to distinguish in observer output.

### Branch exhaustiveness
There is no compile-time enforcement that branch cases cover all possible inputs. Unmatched inputs produce a runtime `contract_violation` error.

### Parallel/async backends
Only the sequential runtime (`pb::runtime::sequential`) is supported. Parallel backends (thread-pool, oneTBB, Taskflow, stdexec) remain roadmap work.

### Stable graph export
DOT/JSON helper output exists for supported branch shapes, and JSON now reports branch topology for branch pipelines. Stable descriptor/export schemas, release-grade golden compatibility fixtures, CLI/file export, and backend graph-export semantics are not yet supported.

## Related pages

- [Branch / Join Roadmap / Status](branch-join-roadmap.md)
- [Production Readiness Status](production-readiness.md)
- [Roadmap Gap Map](roadmap-gap-map.md)
