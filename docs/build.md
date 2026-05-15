# Build and Verification

The project uses CMake 3.23+ and C++20. Prefer Ninja and Clang for local development.

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja
```

`PB_ENABLE_CLANG_TIME_TRACE=ON` is available through the `clang-time-trace` preset for compile-time profiling. Optional backend flags are present but intentionally off in the base scaffold.

## Benchmark smoke targets

Benchmark targets are intentionally opt-in so the default developer presets stay fast. Enable them on an existing preset when you want a local smoke check for header inclusion cost or basic sequential runtime overhead:

```bash
cmake --preset clang-dev-ninja -DPB_BUILD_BENCHMARKS=ON
cmake --build --preset clang-dev-ninja --target pb_bench_include_pipeline
cmake --build --preset clang-dev-ninja --target pb_bench_sequential_5_stage
./build/clang-dev-ninja/bench/pb_bench_include_pipeline
./build/clang-dev-ninja/bench/pb_bench_sequential_5_stage
```

Use `pb_bench_include_pipeline` as a guard for the public-header include surface. Use `pb_bench_sequential_5_stage` as a smoke check that the runtime benchmark scaffold still exercises the sequential executor. These targets are not pass/fail performance gates yet; record timings alongside compiler, build type, and preset before comparing results across machines.

For compile-time profiling, build the same translation units with the Clang time-trace preset:

```bash
cmake --preset clang-time-trace -DPB_BUILD_BENCHMARKS=ON
cmake --build --preset clang-time-trace --target pb_bench_include_pipeline
cmake --build --preset clang-time-trace --target pb_bench_sequential_5_stage
find build/clang-time-trace -name '*.json' -path '*CMakeFiles*' -print
```

Treat the generated trace files as local diagnostics. They are useful for spotting expensive headers and template instantiations, but they should not be committed unless a future release process explicitly asks for captured benchmark artifacts.
