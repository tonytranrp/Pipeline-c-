# pb.core.graph.v1 — formal export schema

**Status:** stable. Producers and consumers may rely on the schema described
on this page across all v1.x releases of Pipeline-c++.

This page formalizes the JSON, DOT, and compact-text helper output produced
by `pb::to_json<Pipeline>()`, `pb::to_dot<Pipeline>()`, and
`pb::to_text<Pipeline>()`. It supersedes the historical "helper schema"
caveats in `docs/export-helper-schema.md` for the v1.x line.

## Version policy

- `schema_version` is the contract identifier. Producers MUST emit the exact
  literal string `"pb.core.graph.v1"` in JSON output and in the leading
  comment of compact-text output.
- The major component (`v1`) MUST NOT change unless a breaking field
  rename, field removal, or semantic redefinition lands. Such a change
  bumps the schema to `pb.core.graph.v2` and the existing v1 helpers are
  retained alongside the new emitter.
- New optional fields MAY be added at the end of an object without bumping
  the major. Consumers MUST ignore unknown fields rather than failing.
- Field order in JSON output is stable for a given pipeline shape — this
  enables byte-equal regression tests. The order is documented below.
- The `pb::runtime::descriptor_schema_version` constant (`pb.descriptor.v1`)
  is a sibling contract that governs the in-memory descriptor records.
  Bumps to either schema are independent.

## Topology values

`topology` takes one of three string values:

- `"linear"` — straight chain of stages.
- `"branch"` — one branch stage that selects exactly one passing case.
- `"fan_in"` — one branch stage that runs every passing case and merges
  results through a join.

## JSON schema (top-level)

```json
{
  "schema_version": "pb.core.graph.v1",
  "topology": "linear" | "branch" | "fan_in",
  "stage_count": <unsigned>,
  "edge_count": <unsigned>,
  "stages": [ <StageRecord>, ... ],
  "edges":  [ <EdgeRecord>, ... ]
}
```

Field order in emitted output is: `schema_version`, `topology`,
`stage_count`, `edge_count`, `stages`, `edges`.

### StageRecord

```json
{
  "index": <unsigned>,
  "key":   "<stable identifier>",
  "name":  "<human-readable label>",
  "kind":  "stage" | "branch" | "fan_in",
  "branch_cases": [ <BranchCaseRecord>, ... ]   // only when kind != "stage"
}
```

- `index` matches the position in the `stages` array.
- `key` is the value of `Stage::stage_key()` or the type-derived fallback.
- `name` is the value of `Stage::stage_name()` or the type-derived fallback.
- `kind` distinguishes ordinary stages from branch and fan-in topology nodes.
- `branch_cases` is omitted when `kind == "stage"`. For `branch` and
  `fan_in` stages it contains one record per case in declaration order.

### BranchCaseRecord

```json
{
  "index": <unsigned>,
  "case_id":           "<stable per-case id>",
  "case_key":          "<stable per-case key>",
  "case_label":        "<user-supplied label or stringified index>",
  "predicate_node_id": "<graph-local predicate node id>",
  "stage_node_id":     "<graph-local case stage node id>",
  "predicate_key":     "<predicate stage key>",
  "predicate_name":    "<predicate stage name>",
  "stage_key":         "<case stage key>",
  "stage_name":        "<case stage name>",
  "predicate_edge":    { "from": "branch",    "to": "predicate",  "style": "dashed", "label": "test" },
  "stage_edge":        { "from": "predicate", "to": "case_stage" }
}
```

The two embedded edge objects are emitted as literal helper-rendered
records and form part of the v1 contract.

### EdgeRecord

```json
{
  "index": <unsigned>,
  "from_stage_index": <unsigned>,
  "to_stage_index":   <unsigned>,
  "from_key":  "<source stage key>",
  "from_name": "<source stage name>",
  "to_key":    "<destination stage key>",
  "to_name":   "<destination stage name>"
}
```

## DOT schema

DOT output is wrapped in `digraph <graph_name>` with `rankdir=LR` and
`node [shape=box]`. For linear pipelines the emitter produces stage
nodes labelled `<name>\n(<key>)` and edges labelled `<from_key> -> <to_key>`.
For branch / fan-in pipelines the emitter additionally adds:

- a `from_input` node,
- a `to_output` node (rendered as `doublecircle`),
- a `branch_<index>` diamond node,
- a `cluster_case_<branch_index>_<case_index>` subgraph per case,
- a predicate node labelled `pred: <predicate name>`,
- a case-stage node labelled with the stage name,
- a dashed `branch -> predicate` test edge,
- a plain `predicate -> case_stage` edge.

The graph name follows DOT identifier rules. The export helper sanitizes
non-identifier characters to `_`.

## Compact text schema

The compact text format is human-grep-friendly and is part of the v1
contract. It must be parseable line-by-line.

```
# pb.core.graph.v1  topology=<linear|branch|fan_in>  stages=<N>  edges=<M>
[<i>] <stage_key>  "<stage_name>"
    case <j>  predicate=<predicate_key>  stage=<stage_key>     (branch/fan_in only)
edges:
  <from_index> -> <to_index>  <from_key> -> <to_key>
```

Rules:

- The first line is the schema banner. Its prefix `# pb.core.graph.v1` is
  the canonical version marker.
- Stage lines begin with `[<index>] ` and are followed by the stage key.
- Stage names are only emitted (in double quotes) when they differ from
  the key, keeping output compact for pipelines that don't bother to set
  a separate display name.
- Case lines are indented four spaces and use the `predicate=<k>` and
  `stage=<k>` keyword form.
- The `edges:` block is omitted entirely when `edge_count == 0`.

## Conformance tests

Conformance is regression-tested in:

- `tests/compile_pass/export_golden.cpp` — locked byte-equal JSON and DOT
  output for representative linear and branch pipelines.
- `tests/compile_pass/export_text_golden.cpp` — locked byte-equal compact
  text output across all three topologies.
- `tests/compile_pass/schema_v1_contract.cpp` — verifies the literal
  `schema_version` string MUST remain `"pb.core.graph.v1"` across builds.

A change that perturbs any of those tests is a schema break. Either the
change is wrong, or the major must be bumped.

## Migration to v2 (future)

When v2 lands:

1. Add `to_json_v2`, `to_dot_v2`, `to_text_v2` next to the v1 emitters.
2. Keep v1 emitters and golden tests as a regression net.
3. Document the diff in this directory under `export-schema-v2.md`.
4. Update `pb_cli schema` to advertise both versions.

Consumers that need to detect schema bumps SHOULD parse the
`schema_version` field of JSON output or the banner of text output and
fall back gracefully when the major changes.
