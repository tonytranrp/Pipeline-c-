# Production Readiness Status

This project is still an MVP foundation for the roadmap in `research/pipeline_builder_cpp_research_plan.md`. Treat the current tree as production-readiness scaffolding rather than a stable release.

## Supported today

- C++20 target-based CMake project with `pb::core`, `pb::runtime`, and the `pb::pipeline` compatibility target.
- Standard-library-only public core/runtime surfaces unless an optional backend is explicitly enabled in a future slice.
- Linear typed pipeline validation with `pb::from<T>::then<S>::to<U>`.
- Stage metadata through explicit `input_type`, `output_type`, `error_type`, and `stage_name()` or adapter-provided traits.
- Public pipeline introspection helpers: `pipeline_size_v`, `pipeline_input_t`, `pipeline_output_t`, `pipeline_stage_t`, `pipeline_stage_descriptor_t`, `describe()`, and stage records.
- Free-function and function-object adapters for legacy code integration.
- Sequential runtime execution for validated linear pipelines.
- Compile-pass, compile-fail, runtime, example, package, and benchmark smoke scaffolding.

## Local readiness checks

Run the developer build and test loop before relying on the tree:

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja --output-on-failure
```

When touching examples or benchmark guidance, also run the relevant smoke targets from [Build and Verification](build.md). Record the preset, compiler, and build type with any benchmark timing because the benchmark targets are not release performance gates yet.

## Package consumer smoke

The installed package is expected to support downstream CMake consumers that use:

```cmake
find_package(pipebuilder CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE pb::pipeline)
```

`pb::pipeline` is the documented compatibility target for consumers. It forwards to the installed runtime target so users do not need to know whether the implementation is split between `pb::core` and `pb::runtime`.

The package release preset in [Build and Verification](build.md) is the release-readiness gate for this contract. The `pb_package_config_smoke` test installs the current build into a temporary prefix, configures a separate consumer with `find_package(pipebuilder CONFIG REQUIRED)`, links that consumer to `pb::pipeline`, and builds it. Treat that test as the minimum package-consumer contract.

## Release checklist

Before cutting a release candidate, collect evidence for:

- `cmake --list-presets=all` succeeds and the preset names are unique.
- Debug and release Clang package presets configure, build, and pass CTest.
- `pb_package_config_smoke` passes in the release package preset.
- Benchmark smoke targets run, and any recorded numbers include the context listed in [Build and Verification](build.md).
- Public examples still build/run, including the intentional compile-fail diagnostic example.
- Known gaps below are either documented for the release or converted into blocking issues.

## Known gaps before a stable release

- Branch, join, graph export, observer hooks, and optional backend execution are roadmap items, not current guarantees.
- Public diagnostics are covered by compile-fail smoke tests, but the exact diagnostic wording is still being hardened.
- Runtime error propagation exists for the current sequential path; richer exception policies, `std::expected` integration, and observer/error-category APIs are future slices.
- Benchmark scaffolding can prove the targets build and run, but release thresholds and CI regression budgets are not established.
- Cross-compiler validation beyond the local configured toolchain remains a release gate.

## Documentation contract

Examples should show one complete successful path and one intentional failure path. If an example relies on future roadmap behavior, label it as aspirational instead of presenting it as supported. Keep public API promises aligned with the tests that currently pass.
