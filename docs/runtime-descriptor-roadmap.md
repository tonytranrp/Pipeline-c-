# Runtime Descriptor Roadmap / Status

An exportable runtime descriptor is **not** a supported feature in the current tree. Treat this page as a roadmap/status note for a stable runtime-facing graph/descriptor surface, not as public API documentation.

## Current status

Today the repository supports:

- compile-time pipeline metadata/introspection through `describe()` and stage records
- public `pipeline_descriptor` metadata for validated linear pipelines
- sequential runtime execution for the current MVP linear pipeline model

Today the repository does **not** support:

- a stable runtime descriptor or runtime graph view that is documented as public supported output
- public APIs that lower pipeline metadata into a durable runtime export format
- descriptor-backed DOT/JSON export support
- descriptor-specific examples, tests, or compatibility guarantees

Keep release notes and examples aligned with that boundary. Descriptor alias symmetry, linear descriptor/error/observer identity checks, and `pb::runtime::error_record` strengthen current linear introspection/diagnostic projections, but they do not create a stable runtime descriptor/export schema or runtime graph view.

## Stable descriptor schema for the current linear MVP

The repository already exposes a small, fixed descriptor shape for validated linear pipelines. That shape is the current stable schema for compile-time-oriented descriptor consumers:

### `pb::core::stage_record`

| Field | Type | Meaning |
| --- | --- | --- |
| `index` | `std::size_t` | Zero-based stage position in pipeline order. |
| `key` | `std::string_view` | Stage key reported by `stage_traits`. |
| `name` | `std::string_view` | Stage display name reported by `stage_traits`. |

### `pb::core::edge_record`

| Field | Type | Meaning |
| --- | --- | --- |
| `index` | `std::size_t` | Zero-based edge position between adjacent stages. |
| `from_stage_index` | `std::size_t` | Source stage index for the edge. |
| `to_stage_index` | `std::size_t` | Target stage index for the edge. |
| `from_key` | `std::string_view` | Source stage key. |
| `from_name` | `std::string_view` | Source stage name. |
| `to_key` | `std::string_view` | Target stage key. |
| `to_name` | `std::string_view` | Target stage name. |

### `pb::core::pipeline_descriptor_view<StageCount>`

The view is a fixed-size aggregate with:

- `stage_count == StageCount`
- `edge_count == StageCount > 0 ? StageCount - 1 : 0`
- `empty == (StageCount == 0)`
- `stages` as an `std::array<stage_record, StageCount>`
- `edges` as an `std::array<edge_record, edge_count>`

The view preserves pipeline order. Stage records and edge records are indexed in the same linear order as the validated pipeline.

### Stability rules

- The schema is linear only; it models adjacent stage-to-stage relationships.
- Field names and ordering are intentionally fixed so docs, tests, and tooling can refer to them consistently.
- Keys and names are borrowed views; they reflect stage metadata and are not owning strings.
- The schema is suitable for compile-time metadata inspection and deterministic test assertions.
- This schema does **not** imply a stable runtime export format, graph serialization contract, or cross-release ABI promise.

## Why an exportable runtime descriptor matters

The research plan treats an exportable runtime descriptor as the boundary between template-heavy compile-time metadata and tooling/runtime consumers that need a stable representation. That kind of descriptor would eventually support:

- graph export to human-readable formats such as DOT or JSON
- stable tooling integration without exposing internal template structure
- runtime graph inspection and visualization
- future backend lowering or trace/reporting surfaces that should not depend on raw compile-time internals

Without that descriptor layer, current introspection remains useful but intentionally limited to compile-time-oriented metadata helpers.

## Intended scope relative to the research plan

The research plan treats the feature as planned work, not current behavior:

- it explicitly says the graph should be exportable at compile time into a runtime descriptor
- it sketches future `export_dot` / `export_json` usage built on descriptor-style metadata
- it warns that graph export should avoid leaking unstable internal template details
- it sketches a future runtime graph/view shape separate from the current public compile-time descriptor helpers

The current repository documents the direction, but it does not yet ship the stable runtime-facing representation or validation needed to claim support.

## Non-goals for the current MVP

The current MVP should **not** claim:

- a stable runtime descriptor ABI or schema
- guaranteed descriptor compatibility across releases
- public export APIs built on today’s compile-time metadata alone
- a runtime graph view that tools can depend on as supported surface

Those decisions belong to later slices with explicit API definitions, tests, and export contracts.

## What would be required before claiming support

Before a runtime descriptor can move from roadmap to supported behavior, the repo needs:

1. **A defined public descriptor surface**  
   Stable names, fields, ownership/lifetime rules, and compatibility expectations.
2. **Lowering from validated pipeline metadata**  
   A supported path from compile-time descriptors/stage records into the runtime descriptor without exposing unstable implementation details.
3. **Targeted tests and examples**  
   Positive checks for descriptor shape/export paths plus negative checks for unsupported graphs or unstable metadata.
4. **Tooling/export integration**  
   Clear linkage to graph export or visualization consumers once those features exist.
5. **Documentation and compatibility guidance**  
   Explicit notes about what is stable, what is versioned, and what remains experimental.

## Verification status today

The current verification evidence covers the existing compile-time metadata, linear descriptor/error/observer identity, and sequential runtime core, not a stable runtime descriptor feature:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

There is currently no runtime-descriptor-specific implementation or verification target under `tests/`, `examples/`, or `bench/`.

## Release guidance

Until implementation and tests land, release notes and docs should describe a runtime descriptor as:

> roadmap work only; unimplemented and unverified as a stable supported feature in the current tree

If a future slice adds a runtime descriptor, update this page together with:

- `docs/production-readiness.md`
- graph-export docs that depend on descriptor-backed export
- examples/tests that prove the descriptor shape and stability contract
