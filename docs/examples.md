# Examples

## Success example

Run the example target to confirm the repository builds a minimal executable:

```bash
./build/examples/pb_basic
```

## Failure example

The compile-fail test demonstrates the expected diagnostic surface for edge mismatches:

```bash
cmake --build build --target pb_compile_fail_edge_mismatch
```

