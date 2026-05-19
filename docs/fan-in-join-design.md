# Fan-in Join Design

This document describes the all-branches fan-in join design for branch
pipelines and records which parts are implemented today. The first sequential
runtime slice is now implemented through `::fan_in<JoinStage>` and
`::join_all<JoinStage>`. Backend/parallel scheduling, richer failed-case
aggregation, stable descriptor/export topology, and long-term schema
compatibility remain future work.

## Current selected-output join

The current branch surface is a selected-output join:

```cpp
pb::from<Input>
  ::branch<CaseA, CaseB, CaseC>
  ::join<JoinStage>
  ::to<Output>;
```

Runtime semantics are first-match-wins:

1. Evaluate branch predicates in case order.
2. Select the first predicate that returns true.
3. Execute only the selected case stage.
4. Pass that one selected case output to the join stage.

For homogeneous case outputs, the branch output is the shared output type. For
heterogeneous case outputs, the branch output is a `std::variant<...>` whose
alternatives preserve case order, including duplicate output types by index.
A selected-output join may also declare `pb::meta::type_list<...>` input so the
runtime dispatches the selected variant alternative to a matching overload.

This is not all-branches fan-in. Non-selected cases are not executed and do not
produce values for the join.

## All-branches fan-in goal and current status

All-branches fan-in is an explicit, separate feature. It executes a set of
branch cases, collects each case outcome in deterministic case order, and
invokes a join stage with a complete fan-in value model. The implemented first
slice is sequential and records `skipped` / `completed` case states; richer
failed-case aggregation remains a follow-up policy.

The design goals are:

- keep current `::branch<...>::join<...>` selected-output behavior unchanged
- make fan-in opt-in and visually distinct in source code
- preserve deterministic case ordering for types, runtime values, observer
  events, diagnostics, descriptors, and future export helpers
- support zero, one, or many passing predicates without ambiguity
- define error aggregation before adding parallel backends
- keep move-only input rules explicit and testable

## Proposed explicit DSL

Keep the current selected-output DSL:

```cpp
pb::from<Input>::branch<CaseA, CaseB>::join<SelectedJoin>::to<Output>;
```

The implemented all-branches fan-in spelling is:

```cpp
pb::from<Input>::branch<CaseA, CaseB>::fan_in<FanInJoin>::to<Output>;
```

with `join_all<JoinStage>` as a source-compatible alias:

```cpp
pb::from<Input>::branch<CaseA, CaseB>::join_all<FanInJoin>::to<Output>;
```

`fan_in<JoinStage>` remains the preferred spelling because it names the
topology rather than overloading the existing selected-output `join` concept.
Documentation must keep `join` and `join_all` sharply separated.

Do not make `::join<...>` switch behavior based on the join stage input type.
That would make source-compatible code silently change semantics when a join
stage is refactored.

## Predicate and branch execution semantics

A fan-in branch evaluates every predicate in declaration order. Each case then
enters one of these states:

- `skipped`: predicate returned false
- `completed`: predicate returned true and the case stage produced a value
- `failed`: predicate or case stage returned/raised an error

For the first implementation slice, sequential fan-in evaluates and runs cases
in declaration order. Later backends may schedule passing cases in parallel, but
observable result ordering must still follow declaration order.

Suggested sequential algorithm:

1. Notify branch start.
2. For each case in order:
   - notify case scheduled/evaluating
   - run the predicate against the branch input
   - if false, record `skipped`
   - if true, run the case stage and record `completed` or `failed`
3. If errors exist, apply the error aggregation policy.
4. Invoke the fan-in join stage with the ordered aggregate.
5. Notify join start/completion or failure.

The first slice should not short-circuit on the first successful predicate.
The current first slice returns on predicate/stage failure before invoking the
join. Richer aggregation that keeps multiple failed cases visible in one
aggregate remains a policy follow-up.

## Zero-pass behavior

Zero passing predicates is a valid fan-in state, not an implicit contract
violation. The join receives an aggregate where every case is `skipped`.

A user can reject zero-pass explicitly in the join stage. This keeps the fan-in
runtime generic and lets domain code decide whether an empty fan-in is an error.

A future policy may offer `require_one_match`, but it should be opt-in and
should report a clear contract error before the join runs.

## Error aggregation

Fan-in needs an ordered error model because more than one case can fail.

Recommended first-slice behavior:

- collect predicate and case-stage errors in case order
- do not invoke the join if any case is `failed`
- return a fan-in aggregate error that contains:
  - branch stage id
  - ordered per-case errors
  - case index
  - predicate/stage role
  - original error category and message

If the current public `pb::runtime::error` cannot carry multiple nested errors,
start with a single `contract_violation` or stage error whose message summarizes
all failing case indexes, and keep a richer `fan_in_error` design as the next
slice. Do not lose deterministic ordering.

## Ordering rules

Fan-in ordering is declaration order everywhere:

- predicate evaluation order in the sequential backend
- aggregate value tuple/type-list order
- observer event case indexes
- diagnostic case indexes
- descriptor/export case indexes
- error aggregation order

Parallel backends may execute work concurrently, but they must publish results
and errors in declaration order.

## Type model

The fan-in join should receive one ordered aggregate with one slot per case.
The preferred type model is a tuple-like aggregate of case outcomes:

```cpp
pb::fan_in_results<
  pb::fan_in_case_result<0, CaseAOutput>,
  pb::fan_in_case_result<1, CaseBOutput>,
  pb::fan_in_case_result<2, CaseCOutput>>
```

Each case-result should represent:

- skipped predicate
- completed value
- failed error

A possible implementation shape is:

```cpp
template <std::size_t Index, class T>
struct fan_in_case_result {
  static constexpr std::size_t index = Index;
  fan_in_case_state state;
  std::optional<T> value;
  std::optional<pb::runtime::error> error;
};
```

For move-only outputs, avoid requiring copyable `std::optional<T>` operations in
public examples and tests. A custom storage wrapper or `std::variant` state may
be needed if optional ergonomics become restrictive.

Raw type metadata should remain available as an ordered
`pb::meta::type_list<CaseAOutput, CaseBOutput, ...>` so compile-time validation
can verify that the join accepts the fan-in aggregate.

The join should not receive a `std::variant<...>` because variant represents one
selected output, not all branch outcomes. The join should not receive only a
plain tuple of values because skipped and failed cases need representation.

## Move-only input rules

Selected-output branch execution can move the input into the one selected case
stage. Fan-in cannot move the same input into multiple case stages.

Recommended rules:

- predicates should inspect `const input_type&`
- if more than one case can execute, case stages should accept `const input_type&`
  or an explicitly cloned/projected value
- move-only fan-in input should be rejected unless the branch declares a cloning,
  projection, or shared-view policy that can provide each passing case with a
  valid input
- diagnostics must explain that all-branches fan-in cannot consume one move-only
  input by value in multiple cases

A future `fan_in_with<ProjectionPolicy>` may support per-case projections, but
that should not be part of the initial fan-in API.

## Observer events

Current branch observers expose generic stage events plus case selected/skipped
for selected-output joins. Fan-in needs more explicit lifecycle hooks.

Recommended event set:

- branch started
- case scheduled
- predicate started/completed/failed
- case skipped
- case started
- case completed
- case failed
- join started
- join completed
- join failed
- branch completed
- branch failed

If adding new observer methods is too disruptive, add them through trace event
kinds first and adapt the default observer interface in a compatibility slice.
Event payloads should include branch stage id, case index, predicate id, stage
id, and the deterministic case result state.

## Backend plan

Implementation should be staged by backend:

1. Sequential fan-in is implemented.
   - deterministic predicate and case execution
   - ordered aggregate values for skipped/completed cases
   - runtime tests for zero/one/many pass cases
2. Thread-pool fan-in later.
   - schedule passing case stages concurrently only after sequential semantics
     are fixed
   - preserve declaration-order aggregate output
   - define cancellation/error policy before enabling parallel failure paths
3. Export/backend graph semantics after runtime behavior is stable.
   - descriptor/export helpers may show fan-in topology later
   - do not claim stable graph schema as part of the first runtime slice

## Compile-fail diagnostics

Targeted compile-fail coverage should include:

- fan-in join stage does not accept the fan-in aggregate
- fan-in join output does not match `.to<Output>`
- predicate output is not bool-convertible
- case input is incompatible with the fan-in input policy
- move-only input is consumed by value by multiple possible cases without a
  clone/projection policy
- `fan_in` / `join_all` used without a preceding branch
- selected-output `join` and fan-in APIs mixed in the same branch node
- empty fan-in case list if empty branches remain unsupported
- duplicate case labels or descriptor identities when labels are introduced

Diagnostics should name the branch case index and whether the failure is in the
predicate, case stage, aggregate type, or join stage.

## Runtime tests

Add runtime tests in small slices:

- zero predicates pass and join receives all-skipped aggregate
- one predicate passes and join sees exactly one completed case
- multiple predicates pass and join sees declaration-order completed cases
- predicate failure records the failing case index
- case-stage failure records the failing case index
- mixed skipped/completed/failed ordering is deterministic
- observer events occur in the documented order for sequential fan-in
- move-only input by-value fan-in is rejected at compile time
- join is not invoked when the initial error aggregation policy says errors are
  fatal

After thread-pool support exists, add scheduling tests that prove result order is
stable even when completion order differs.

## Implementation checklist

1. Add design-only docs and keep current selected-output docs unchanged.
2. Introduce internal fan-in marker types separate from selected-output branch
   nodes.
3. Add compile-time validation for fan-in join input/output compatibility.
4. Add compile-fail tests for invalid fan-in DSL and type contracts.
5. Implement sequential fan-in execution with ordered case results.
6. Add observer/trace fan-in event coverage.
7. Add runtime tests for zero/one/many pass cases and error aggregation.
8. Document move-only input restrictions and diagnostics.
9. Only after sequential semantics stabilize, design thread-pool scheduling.
10. Only after runtime semantics stabilize, update descriptor/export helpers and
    schema docs for fan-in topology.

## Non-goals for the current wave

- no backend implementation
- no descriptor/export helper field changes for fan-in topology
- no stable graph schema claim
- no rich multi-error fan-in aggregation object yet
- no clone/borrow/shared-view policy for non-copyable fan-in inputs yet
- no void-output fan-in case aggregation yet
