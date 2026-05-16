# Graph Export Roadmap / Status

Full graph export is **not** a supported feature in the current tree. Treat this page as a roadmap/status note for planned DOT/JSON export work and the narrow linear-DOT coordination boundary, not as broad user-facing graph-export documentation.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time pipeline metadata/introspection via `describe()` and stage records
- sequential runtime execution for validated linear pipelines
- leader-accepted evidence for a narrow **linear DOT** export/helper slice, pending final candidate integration checks before release wording promotes it

Today the repository does **not** support:

- JSON export APIs
- full graph-export runtime helpers or file emitters
- graph-export examples, tests, or benchmark coverage
- branch/join graph lowering required for richer non-linear export surfaces

Keep examples and release notes aligned with that boundary. A linear DOT helper is narrower than full graph export: it does not provide branch/join graph shapes, JSON output, stable schema guarantees, or file-emitter coverage by itself. Branch marker aliases and descriptor alias symmetry do not provide those graph-export guarantees either.

## Linear DOT boundary

The accepted DOT slice is a linear-pipeline visualization/export aid only. Release notes may mention it only when the final candidate includes the implementation and targeted DOT evidence, and they must keep the wording scoped to:

- linear pipeline structure, not branch/join graph topology
- DOT output only, not JSON export or a versioned graph schema
- helper/API behavior covered by targeted tests, not a general file-emitter framework
- current descriptor metadata, not a stable runtime descriptor/export contract

## Why this remains roadmap-only

The research plan treats graph export as planned follow-on work rather than current behavior:

- `F010` / `F011` mark DOT and JSON graph export as `v0.2` features.
- Tasks `111`-`120` describe the future graph-export implementation/test/diagnostic slice.
- Tasks `411`-`420` describe future documentation/example work that depends on graph export existing first.

The current code and verification surfaces still focus on the MVP linear pipeline:

- public composition remains `then` / `to`
- runtime coverage targets sequential execution only
- examples cover the basic order pipeline and one compile-fail diagnostic path
- DOT evidence is treated as a narrow linear helper until final release-candidate verification is attached

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

The current verification evidence proves the existing pipeline core and, where integrated, only the narrow linear-DOT helper:

- compile-pass coverage
- compile-fail diagnostic coverage
- runtime smoke coverage
- package-consumer smoke coverage
- benchmark smoke scaffolding

There is still no evidence for JSON export, branch/join graph export, stable graph schemas, graph-export examples, or benchmark coverage.

## Release guidance

Until full implementation and tests land, release notes and docs should describe broad graph export as:

> roadmap work only; unimplemented and unverified in the current tree

If the final candidate includes the linear DOT slice, describe it separately as:

> linear DOT helper/export only; not JSON export, not branch/join graph export, and not a stable graph schema

If a future slice adds graph export, update this page together with:

- `docs/production-readiness.md`
- `docs/examples.md`
- the relevant tests/examples/bench entries
- release verification notes in `docs/build.md` when new checks are introduced
