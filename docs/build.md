# Build and Verification

The project uses CMake 3.23+ and C++20. Prefer Ninja and Clang for local development.

```bash
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja
```

`PB_ENABLE_CLANG_TIME_TRACE=ON` is available through the `clang-time-trace` preset for compile-time profiling. Optional backend flags are present but intentionally off in the base scaffold.
