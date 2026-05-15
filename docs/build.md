# Build and Verification

The project uses CMake 3.23+ and C++20. Prefer Ninja and Clang for local development.

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja
```

`PB_ENABLE_CLANG_TIME_TRACE=ON` is available through the `clang-time-trace` preset for compile-time profiling. Optional backend flags are present but intentionally off in the base scaffold.

Preset quick map:

- `dev-ninja`: GNU Debug baseline with tests/examples.
- `clang-dev-ninja`: Clang Debug baseline.
- `clang-tidy-ninja`: enables `PB_ENABLE_CLANG_TIDY`.
- `clang-time-trace`: enables compile-time trace (`PB_ENABLE_CLANG_TIME_TRACE`).
- `release-ninja`: release build defaults.
- `bench-dev-ninja`: benchmarks-only build (`PB_BUILD_BENCHMARKS=ON`, tests/examples off).
- `package-dev-ninja`: package-oriented debug lane with `CPACK_GENERATOR=TGZ`.
