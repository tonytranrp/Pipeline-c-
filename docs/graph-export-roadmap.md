# Graph Export Roadmap / Status

Graph export is now a **partial helper surface**, not a stable graph interchange contract. The current tree has linear DOT support, branch-aware DOT output, and JSON output that distinguishes `"topology":"linear"` from `"topology":"branch"` when branch stages are present.

Keep release notes scoped to that helper boundary: the repository still does not provide descriptor-backed stable export schemas, CLI export of user pipeline files, or backend/parallel graph export. Current golden-style tests are helper-output regression tests, not a compatibility promise for external consumers.

## Current status

Today the repository supports:

- linear compile-time pipeline validation through `pb::from<T>::then<S>::to<U>`
- compile-time pipeline metadata/introspection via `describe()` and stage records
- sequential runtime execution for validated linear and supported branch pipelines
- DOT export helpers for linear pipelines and branch/case structure
- JSON export helpers for linear pipelines and branch pipelines, including branch topology detection and branch case metadata

Today the repository does **not** support:

- a stable descriptor-backed DOT/JSON compatibility contract
- golden schema fixtures or field-ordering guarantees for long-term consumers beyond helper-output regression tests
- graph-export file emitters or parsing of user pipeline definitions through the CLI
- multi-input join graph lowering
- parallel/backend graph execution export semantics

## Supported helper boundary

The current export helpers may be used as local visualization and smoke-test aids. They are intentionally narrower than a release-grade graph schema:

- branch pipelines now report branch topology in JSON instead of being mislabeled as linear
- DOT can show branch/case structure for the supported branch slice, with label escaping for quotes, backslashes, tabs, carriage returns, and newlines
- JSON/DOT and descriptor generation are still separate paths, so docs must not claim descriptor-backed export yet
- exported strings are covered by targeted compile-pass checks and helper-output golden regressions, not by a versioned schema contract

## Why full graph export remains roadmap work

The research plan treats graph export as a staged follow-on:

- `F010` / `F011` mark DOT and JSON graph export as `v0.2` features.
- Tasks `111`-`120` describe the future graph-export implementation/test/diagnostic slice.
- Tasks `411`-`420` describe future documentation/example work that depends on graph export existing first.

The current implementation is useful progress, but it is still not the final architecture. The next stabilizing direction is:

```text
Pipeline type -> descriptor v1 -> DOT/JSON exporters
```

That keeps DOT, JSON, and descriptor output from drifting as graph features grow.

## Planned implementation checkpoints

When stabilizing graph export, keep it staged and separately verifiable:

1. **Make descriptor the source of truth**
   Lower pipeline types into descriptor v1, then render DOT/JSON from that descriptor.
2. **Document schema and compatibility**
   Add field-ordering expectations, branch edge labels, type-name fields, and a schema changelog.
3. **Promote golden verification into a compatibility suite**
   Existing helper-output golden regressions cover linear JSON, homogeneous branch JSON, heterogeneous branch JSON, and branch DOT. A future stable export phase should add descriptor-backed golden DOT/JSON/descriptor fixtures, schema version docs, and compatibility review rules.
4. **Expose file/CLI behavior only after tests exist**
   Keep CLI/export examples scoped until user-pipeline parsing and export are implemented.

## Verification status today

Current targeted evidence includes:

- `pb_export_dot_compile_pass`
- `pb_export_dot_branch_compile_pass`
- `pb_export_json_compile_pass`
- `pb_export_json_branch_compile_pass`
- `pb_export_golden_compile_pass` helper-output regression checks
- public-header coverage for export headers

There is still no stable descriptor-backed schema, release-grade golden-output compatibility suite, CLI file-export path, or backend graph-export evidence.

## Release guidance

Release notes and docs may describe the current slice as:

> DOT/JSON helper export for linear and supported branch pipelines, including branch topology in JSON; not a stable descriptor-backed graph schema and not a CLI/file export contract.

If a future slice stabilizes graph export, update this page together with:

- `docs/production-readiness.md`
- `docs/examples.md`
- `docs/runtime-descriptor-roadmap.md`
- the relevant tests/examples/bench entries
- release verification notes in `docs/build.md` when new checks are introduced
