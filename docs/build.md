# Build and Verification

## Configure

```bash
cmake -S . -B build
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

