# Current PR / Release Summary

Status snapshot for the current long-horizon team batch. Use this page as a compact handoff note before copying details into a PR body or release evidence record.

## Candidate snapshot

- Candidate baseline observed before this cleanup: `52e1512` on `main` / `origin/main`.
- Recent integrated hardening commits observed in this worktree:
  - `4be3ab1` / `3e48488` — locked linear descriptor, observer failure callback, and `try_run()` error identity consistency.
  - `371312a` / `218664c` — added runtime `error_record` / `to_record(...)` diagnostic projection and smoke coverage.
  - `c13a54c` / `2939908` — preserved/fill custom expected-like diagnostic stage identity and aligned error records, observer failure/exception stage identity, and `engine.describe()` in the linear runtime path.
  - `30447ae` — required homogeneous `branch_node` case inputs and locked the mismatch diagnostic while staying marker-only.
  - `4c6419e` — added marker-only `branch_case_output` / `branch_outputs` metadata scaffolding and public-header coverage.
  - `5afcb3a` / `f0652be` — added compile-fail diagnostics for branch-output marker misuse.
  - `dbb8d5b` — added marker-only `join_node` validation and invalid join-stage diagnostics.
  - `b737175` — checked branch_case source compatibility at the marker boundary.
  - `7a483b7` — added adapter/member hardening in `include/pb/adapt/fn.hpp`.
  - `ee412f7` — added public-header coverage for core `stage_traits` aliases and tightened related diagnostic misuse cases.
  - `3b1a231` / `23f1d60` — integrated runtime `pb::runtime::result<T>::error_or(...)` fallback selection hardening.
  - `ec45eae` / `62820ed` / merge `caa43ee` — locked sequential observer replacement/accessor behavior with runtime smoke coverage.
- Additional leader-accepted lane evidence after this snapshot:
  - homogeneous sequential branch execution is now integrated and supported for the current standard-library sequential backend, including public branch/join DSL, validation, runtime routing, observer events, stateful storage, examples, and tests.
  - branch output compatibility validation and join consumption validation are part of that supported homogeneous branch/join slice; heterogeneous branch outputs and move-only branch inputs remain roadmap-only.
  - DOT export evidence is scoped to a narrow linear helper; keep JSON, branch/join graph export, and stable graph schema claims out of release text.
- Current cleanup scope: branch child identity implementation/test coverage plus release summary/checkpoint alignment.

## PR summary draft

- Runtime diagnostic records and linear descriptor/observer/error identity checks, including custom expected-like failure and exception stage identity, now strengthen the current sequential diagnostic story; release notes should avoid calling this a stable exported diagnostic schema or runtime descriptor contract.
- Branch/join support now covers source compatibility, predicate shape, homogeneous branch-node case inputs, branch output compatibility validation, join consumption validation, runtime sequential routing, observer case events, stateful branch storage, invalid join-stage markers, and branch-output marker misuse. Release notes must still label heterogeneous branch/join execution, move-only branch inputs, multi-input joins, and graph export as roadmap.
- Linear DOT export/helper evidence may be mentioned only as linear DOT support on a verified candidate; release notes must label the current JSON export as a narrow descriptor/metadata helper and keep branch/join graph export, stable graph schemas, and broad graph export as roadmap.
- Adapter/member hardening landed in the public adapter surface; release notes should keep this as incremental adapter ergonomics/hardening, not a new backend claim.
- Core stage-traits public-header coverage now exercises the alias/metadata surface used by diagnostics and docs.
- Runtime result normalization now has stronger coverage around fallback/error conversion boundaries.
- Sequential observer registration now has smoke coverage for replacing and clearing the observer sink through the public accessor path.
- Roadmap-facing docs should continue to describe observer hooks as partial sequential-runtime support: the current callback path is supported, but ABI, event schema, cross-executor semantics, examples, and benchmark/cost evidence remain follow-on work.


## Docs-lane verification collected

For this docs/checkpoint update, the worker refreshed local developer evidence on the current worktree:

- `cmake --preset clang-dev-ninja` — passed.
- `cmake --build --preset clang-dev-ninja` — passed.
- `ctest --preset clang-dev-ninja --output-on-failure` — passed, `124/124` tests on the current candidate.

Fresh package-release evidence was also collected on the current worktree:

- `cmake --preset package-release-clang-ninja` — passed on a clean reconfigure.
- `cmake --build --preset package-release-clang-ninja` — passed.
- `ctest --preset package-release-clang-ninja --output-on-failure` — passed, `124/124` tests.
- `cmake --build --preset package-release-clang-ninja --target package` — passed and produced `build/package-release-clang-ninja/pipebuilder-0.1.0-Linux.tar.gz`.

This is now fresh package-release evidence on the current worktree, and the full package-release CTest lane is green after the branch identity/docs hardening cleanup.

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
