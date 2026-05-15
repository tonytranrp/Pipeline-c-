# Examples

- `examples/basic_order_pipeline.cpp` shows named stages, adapted legacy functions/functors, `pb::from<T>::then<S>::to<U>`, and sequential execution.
- `examples/error_diagnostic.cpp` can be compiled with `PB_EXAMPLE_ENABLE_COMPILE_FAILURE` to demonstrate the edge-mismatch diagnostic path.
