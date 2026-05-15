# Examples

The examples are intentionally small and mirror the current supported API. Use them as copyable starting points for a linear pipeline with explicit stage types and compile-time validation.

## Successful order pipeline

`examples/basic_order_pipeline.cpp` demonstrates the happy path:

1. Define domain input/output types (`RawText`, `OrderDraft`, `ValidatedOrder`, `Receipt`).
2. Adapt legacy callables with `pb::adapt_fn` or `pb::adapt_functor` when the callable already exists outside the pipeline library.
3. Define any new stage as a type with `input_type`, `output_type`, `error_type`, `stage_name()`, and `operator()`.
4. Compose the chain with `pb::from<T>::then<S>::to<U>`.
5. Guard the chain with `static_assert(pb::valid<Pipeline>)`.
6. Run the validated chain with `pb::compile<Pipeline>(pb::runtime::sequential{})`.

Build and run only the example target during tutorial work:

```bash
cmake --build --preset clang-dev-ninja --target pb_basic_order_pipeline
./build/clang-dev-ninja/examples/pb_basic_order_pipeline
```

A zero exit code means the sequential executor preserved the order id through the adapted parse stage, adapted validate stage, and explicit persist stage.

## Diagnostic failure example

`examples/error_diagnostic.cpp` keeps the invalid pipeline behind `PB_EXAMPLE_ENABLE_COMPILE_FAILURE` so normal example builds stay green. Enable that definition only when you want to inspect the compile-time edge-mismatch diagnostic.

The failing path intentionally connects `ParseOrder`, which outputs `OrderDraft`, to `PersistOrder`, which declares `Receipt` as its input. The expected diagnostic includes `Pipeline edge mismatch`.

Use CTest for the stable failure check instead of hand-running a compiler command:

```bash
ctest --preset clang-dev-ninja --output-on-failure -R pb_example_error_diagnostic_compile_fail
```

## Copying an example into user code

When adapting these examples for an application:

- Keep each stage's declared `input_type` and `output_type` aligned with the neighboring stage.
- Prefer adapters for existing free functions and function objects; write explicit stage types when the stage owns pipeline-specific metadata.
- Keep `static_assert(pb::valid<YourPipeline>)` next to the pipeline alias so type mismatches fail before runtime code is involved.
- Start with `pb::runtime::sequential` before introducing future executor or backend assumptions.
- Treat branch/join graphs, graph export, and optional backends as roadmap items until `docs/production-readiness.md` lists them as supported.
