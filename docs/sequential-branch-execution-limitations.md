# Sequential Branch Execution — Supported Features and Limitations

Sequential branch routing with homogeneous outputs and optional join stages is now supported. This page documents what works today and what remains roadmap work.

## Supported

- **Public DSL**: `pb::from<Input>::branch<CaseA, CaseB>::to<Output>` and `pb::from<Input>::branch<CaseA, CaseB>::join<JoinStage>::to<Output>`
- **Compile-time validation**: join context enforced (must follow `::branch<...>`), type compatibility checked, branch predicates validated, branch output homogeneity enforced
- **Runtime execution**: first-match-wins predicate evaluation with short-circuit, observer events (`on_case_selected`, `on_case_skipped`), error propagation with `[branch]` prefix annotation
- **Stateful storage**: branch predicates and stages respect `pb::runtime::stateful_sequential`, preserving mutable state across multiple pipeline runs
- **Join stages**: `::join<JoinStage>` appends a linear stage after the branch node, validated for type compatibility
- **Observer coverage**: standard observer events (`on_stage_start`, `on_stage_success`, `on_stage_failure`, `on_stage_exception`) plus branch-specific events (`on_case_selected`, `on_case_skipped`)
- **Multiple cases**: any number of branch cases (1+) supported via recursive template evaluation

## Not yet supported

### Heterogeneous branch outputs
All branch cases must produce the same `output_type`. A branch whose cases produce `Parsed` and `Reviewed` (different types) will fail at compile time via `branch_output_validation`. Heterogeneous joins that consume `type_list<Parsed, Reviewed>` or `variant<Parsed, Reviewed>` are not yet implemented.

### Move-only branch inputs
The branch input type must be `std::copy_constructible` because predicates and the selected branch stage both inspect the same input value. Move-only inputs fail at compile time via `selected_branch_node::static_assert`.

### Branch-specific observer event ordering
The observer interface provides `on_case_selected` and `on_case_skipped` events, but the exact ordering relative to other observer events is documented as the current implementation behavior, not a stable contract.

### Unnamed stage identity collisions
When predicates and branch stages do not define `stage_key()` / `stage_name()`, the runtime generates fallback identities from the branch node's `StageIndex`. Multiple unnamed predicates or stages may share the same numeric fallback, making them harder to distinguish in observer output.

### Branch exhaustiveness
There is no compile-time enforcement that branch cases cover all possible inputs. Unmatched inputs produce a runtime `contract_violation` error.

### Parallel/async backends
Only the sequential runtime (`pb::runtime::sequential`) is supported. Parallel backends (thread-pool, oneTBB, Taskflow, stdexec) remain roadmap work.

### Full graph export
Linear DOT export exists, but branch/join graph shapes are not yet represented in DOT or JSON export.

## Related pages

- [Branch / Join Roadmap / Status](branch-join-roadmap.md)
- [Production Readiness Status](production-readiness.md)
- [Roadmap Gap Map](roadmap-gap-map.md)
