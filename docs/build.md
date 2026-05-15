# Build and Verification

## Configure

```bash
cmake -S . -B build -DPB_BUILD_TESTS=ON -DPB_BUILD_EXAMPLES=ON -DPB_BUILD_BENCH=ON
```

## Build

```bash
cmake --build build
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Notes

- C++20 is the baseline.
- The project is intended to work with Clang when available, but this scaffold does not require Clang-only features.
- The compile-fail CTest currently verifies the stable `adjacent pipeline stages are not connectable` diagnostic.
- The benchmark targets are compile/link smoke targets for include cost and 5-/50-stage typed chains, not calibrated performance measurements yet.
