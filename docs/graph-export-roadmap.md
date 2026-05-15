# Graph Export Roadmap / Status

Graph export is **not** a supported feature in the current tree. Treat this page as a roadmap/status note for planned DOT/JSON export work, not as user-facing implementation documentation.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time pipeline metadata/introspection via `describe()` and stage records
- sequential runtime execution for validated linear pipelines

Today the repository does **not** support:

- public `export_dot` / `export_json` APIs
- graph-export runtime helpers or file emitters
- graph-export examples, tests, or benchmark coverage
- branch/join graph lowering required for richer non-linear export surfaces

Keep examples and release notes aligned with that boundary. Do not present graph export as partially shipped just because the research plan already names the feature.

## Why this remains roadmap-only

The research plan treats graph export as planned follow-on work rather than current behavior:

- `F010` / `F011` mark DOT and JSON graph export as `v0.2` features.
- Tasks `111`-`120` describe the future graph-export implementation/test/diagnostic slice.
- Tasks `411`-`420` describe future documentation/example work that depends on graph export existing first.

The current code and verification surfaces still focus on the MVP linear pipeline:

- public composition remains `then` / `to`
- runtime coverage targets sequential execution only
- examples cover the basic order pipeline and one compile-fail diagnostic path

## Planned implementation checkpoints

When graph export work starts, keep it staged and separately verifiable:

1. **Define the export API surface**  
   Add stable public names, formats, and invariants without implying branch/join support that does not exist yet.
2. **Lower current metadata into an exportable descriptor**  
   Reuse validated pipeline descriptors and stage records instead of inventing a second source of truth.
3. **Add positive and negative verification**  
   Cover DOT/JSON export success paths plus boundary failures such as unsupported metadata or unavailable formats.
4. **Document examples only after tests exist**  
   Keep user-facing examples aspirational until compile-pass/runtime coverage lands.

## Verification status today

The current verification evidence proves the existing pipeline core, not graph export:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

There is currently no graph-export-specific implementation or verification target under `tests/`, `examples/`, or `bench/`.

## Release guidance

Until implementation and tests land, release notes and docs should describe graph export as:

> roadmap work only; unimplemented and unverified in the current tree

If a future slice adds graph export, update this page together with:

- `docs/production-readiness.md`
- `docs/examples.md`
- the relevant tests/examples/bench entries
- release verification notes in `docs/build.md` when new checks are introduced
