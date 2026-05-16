# Benchmark and Compile-Time Profiling Workflow

This project treats benchmarks as smoke and profiling scaffolding, not release gates. The current workflow exists to prove that the benchmark targets still build and run, and to give maintainers a repeatable way to capture compile-time trace artifacts when investigating template or header cost.


The release-facing status for benchmark thresholds and compile-time budgets is summarized in [Research Verification Matrix](research-verification-matrix.md).
## Current benchmark surface

When `PB_BUILD_BENCHMARKS=ON`, the repository builds four benchmark-oriented executables under `bench/`:

- `pb_bench_include_pipeline` — compile-time/header-surface smoke for `<pb/pipeline.hpp>`.
- `pb_bench_compile_chain_5` — small compile-time chain smoke.
- `pb_bench_compile_chain_50` — larger compile-time chain smoke.
- `pb_bench_sequential_5_stage` — runtime smoke for the current sequential executor.

These targets are registered as CTest entries with `benchmark` and `smoke` labels, so the dedicated benchmark presets can run them without enabling the normal test suite.

## Presets and build modes

The current preset surface splits benchmark work into two lanes plus a Clang time-trace lane:

| Preset | Build type | Purpose |
| --- | --- | --- |
| `bench-dev-ninja` | `Debug` | Fast local benchmark smoke lane with examples/tests disabled and benchmark targets enabled. |
| `benchmark-ninja` | `Release` | Release-like benchmark smoke lane with examples/tests disabled and benchmark targets enabled. |
| `clang-time-trace` | `RelWithDebInfo` | Clang profiling lane with `PB_ENABLE_CLANG_TIME_TRACE=ON`; add `-DPB_BUILD_BENCHMARKS=ON` when you want trace artifacts for benchmark targets. |

The normal developer presets (`dev-ninja`, `clang-dev-ninja`) keep `PB_BUILD_BENCHMARKS=OFF` by default so the common edit-build-test loop stays fast.

## Benchmark smoke workflow

Use the dedicated benchmark presets when you want the repo's current benchmark executables and CTest smoke entries without the rest of the suite:

```bash
cmake --preset bench-dev-ninja
cmake --build --preset bench-dev-ninja
ctest --preset bench-dev-ninja --output-on-failure

cmake --preset benchmark-ninja
cmake --build --preset benchmark-ninja
ctest --preset benchmark-ninja --output-on-failure
```

Expected outputs today:

- Build directories at `build/bench-dev-ninja/` or `build/benchmark-ninja/`.
- Benchmark executables under the corresponding `build/.../bench/` directory.
- CTest entries named `pb_bench_include_pipeline`, `pb_bench_compile_chain_5`, `pb_bench_compile_chain_50`, and `pb_bench_sequential_5_stage`.
- A smoke-only signal: success means the benchmark translation units and the sequential runtime scaffold still build and run.

If you only need one target during a local investigation, build it directly:

```bash
cmake --build --preset bench-dev-ninja --target pb_bench_include_pipeline
cmake --build --preset bench-dev-ninja --target pb_bench_compile_chain_50
cmake --build --preset bench-dev-ninja --target pb_bench_sequential_5_stage
```

## Compile-time profiling workflow

Use the Clang time-trace lane when you want per-translation-unit JSON traces instead of only a pass/fail smoke signal:

```bash
cmake --preset clang-time-trace -DPB_BUILD_BENCHMARKS=ON
cmake --build --preset clang-time-trace --target pb_bench_include_pipeline
cmake --build --preset clang-time-trace --target pb_bench_compile_chain_5
cmake --build --preset clang-time-trace --target pb_bench_compile_chain_50
find build/clang-time-trace -name '*.json' -path '*CMakeFiles*' -print
```

Expected outputs today:

- A `build/clang-time-trace/` build directory configured with Clang and `PB_ENABLE_CLANG_TIME_TRACE=ON`.
- JSON trace artifacts for compiled translation units under `build/clang-time-trace/**/CMakeFiles/**`.
- The same benchmark executables as the smoke lanes when you build those targets explicitly.

Treat the JSON files as local diagnostic artifacts. They are useful for spotting expensive includes, template instantiations, or unusually heavy translation units, but they are not committed release evidence.

## Recording results

Benchmark and profiling results are only useful when the execution context is captured with them. Record at least:

- Git commit SHA and whether the tree was clean or dirty.
- Preset name, build type, compiler ID/version, generator, and benchmark target.
- Whether the build directory was clean or incremental.
- The exact command used to build or run the target.
- The observed wall time or trace summary and where that number came from.

A short record can look like this:

```text
commit=<sha> tree=<clean|dirty>
preset=bench-dev-ninja target=pb_bench_compile_chain_50 compiler=<id-version>
mode=<clean|incremental> metric=<wall-time|trace-summary> value=<value>
notes=<local context, suspected outlier, or follow-up>
```

## Current limitations and roadmap status

This benchmark lane is still scaffolding:

- The targets are smoke checks, not benchmark-quality performance gates.
- No CI threshold or regression budget is enforced for runtime or compile-time measurements.
- The compile-time targets prove representative translation units build, but they do not establish a complete performance envelope.
- The time-trace workflow is Clang-specific and currently aimed at local diagnosis, not artifact publication.
- Broader performance analysis, stable thresholds, and automated regression handling remain future work in the production-readiness roadmap.

For the current supported benchmark contract, prefer this document plus `docs/build.md`; if those ever disagree, update both in the same change.
