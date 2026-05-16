# Current PR / Release Summary

Status snapshot for the current long-horizon team batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Candidate commit observed by the docs lane: `5afcb3a` / `f0652be` branch-output marker diagnostic integrations plus docs checkpoint `845d2d4` (detached worker integration history).
- Recent integrated hardening commits observed in this worktree:
  - `4be3ab1` / `3e48488` — locked linear descriptor, observer failure callback, and `try_run()` error identity consistency.
  - `371312a` / `218664c` — added runtime `error_record` / `to_record(...)` diagnostic projection and smoke coverage.
  - `4c6419e` — added marker-only `branch_case_output` / `branch_outputs` metadata scaffolding and public-header coverage.
  - `5afcb3a` / `f0652be` — added compile-fail diagnostics for branch-output marker misuse.
  - `dbb8d5b` — added marker-only `join_node` validation and invalid join-stage diagnostics.
  - `b737175` — checked branch_case source compatibility at the marker boundary.
  - `7a483b7` — added adapter/member hardening in `include/pb/adapt/fn.hpp`.
  - `ee412f7` — added public-header coverage for core `stage_traits` aliases and tightened related diagnostic misuse cases.
  - `3b1a231` / `23f1d60` — integrated runtime `pb::runtime::result<T>::error_or(...)` fallback selection hardening.
  - `ec45eae` / `62820ed` / merge `caa43ee` — locked sequential observer replacement/accessor behavior with runtime smoke coverage.
- Docs-lane scope for this note: release summary/checkpoint alignment only; no core/runtime/test implementation.

## PR summary draft

- Runtime diagnostic records and linear descriptor/observer/error identity checks now strengthen the current sequential diagnostic story; release notes should avoid calling this a stable exported diagnostic schema or runtime descriptor contract.
- Branch/join marker diagnostics now cover source compatibility, predicate shape, unsupported topology, invalid join-stage markers, and branch-output marker misuse; release notes must still label branch output routing/compatibility, join consumption, graph export, and executable branch/join topology as roadmap.
- Adapter/member hardening landed in the public adapter surface; release notes should keep this as incremental adapter ergonomics/hardening, not a new backend claim.
- Core stage-traits public-header coverage now exercises the alias/metadata surface used by diagnostics and docs.
- Runtime result normalization now has stronger coverage around fallback/error conversion boundaries.
- Sequential observer registration now has smoke coverage for replacing and clearing the observer sink through the public accessor path.
- Roadmap-facing docs should continue to describe observer hooks as partial sequential-runtime support: the current callback path is supported, but ABI, event schema, cross-executor semantics, examples, and benchmark/cost evidence remain follow-on work.


## Docs-lane verification collected

For this docs/checkpoint update, the worker refreshed local developer evidence on the current worktree:

- `cmake --preset clang-dev-ninja` — passed.
- `cmake --build --preset clang-dev-ninja` — passed.
- `ctest --preset clang-dev-ninja --output-on-failure` — passed, `83/83` tests.

This is developer-preset evidence only. The package-release preset/package target evidence still needs to be collected on the final release candidate before tagging.

## Verification to attach before release tagging

Collect fresh evidence on the final candidate SHA rather than relying on older checkpoints:

```sh
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja --output-on-failure

cmake --preset package-release-clang-ninja
cmake --build --preset package-release-clang-ninja
ctest --preset package-release-clang-ninja --output-on-failure
cmake --build --preset package-release-clang-ninja --target package
```

If a release note mentions observer hooks, explicitly keep the limitation wording: partial sequential support is present; stable observer ABI, event schemas, cross-executor behavior, and tracing/export integrations remain roadmap items.
