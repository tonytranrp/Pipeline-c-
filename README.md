# Pipeline-c++

Compile-time validated C++20 pipeline builder foundation.

This first base follows `research/pipeline_builder_cpp_research_plan.md`:

- target-based CMake project (`pb::core`, `pb::runtime`, plus `pb::pipeline` compatibility target)
- LLVM/Clang-friendly presets and `compile_commands.json`
- public `.hpp` headers under `include/pb/`
- implementation files under `src/`
- examples, tests, docs, and benchmark smoke folders
- MVP `pb::from<T>::then<S>::to<U>` typed linear pipeline validation
- sequential runtime executor and free/function-object adapters

## Docs and release readiness

- [Build and Verification](docs/build.md) lists the supported configure/build/test presets, benchmark smoke targets, and benchmark result recording template.
- [Examples](docs/examples.md) walks through the successful order pipeline, the intentional diagnostic example, and the public introspection helpers.
- [Pipeline-c++ docs hub](docs/README.md) links all guides used during the review lane.
- [Production Readiness Status](docs/production-readiness.md) tracks supported capabilities, known release gaps, and the package-consumer release gate.
- [Research Verification Matrix](docs/research-verification-matrix.md) maps research-plan gaps to shipped evidence, tests, release status, and next slices.
- [Current implementation / not-done map](research/not%20done.md) summarizes what is done, partial, missing, and required for production-grade completion against the original research plan.
- [Graph Export Roadmap / Status](docs/graph-export-roadmap.md) explains the DOT/JSON helper boundary and what must land before docs can present stable graph export as supported.
- [Cross-Compiler Validation Status](docs/cross-compiler-validation.md) records the latest GCC/Clang/MSVC/package validation evidence and remaining release boundaries.

For package consumers, the release-readiness path is the `package-release-clang-ninja` configure, build, CTest, and package target sequence below. That CTest run includes `pb_package_config_smoke`, which installs the package into a temporary prefix, verifies that `find_package(pipebuilder CONFIG REQUIRED)` exposes `pb::core`, `pb::runtime`, and the `pb::pipeline` compatibility target, builds separate downstream consumers against each target, runs those consumers, and checks the generated TGZ for key headers, CMake config files, and the runtime library. Treat the package lane as release evidence only when this preset and package target pass freshly on the candidate SHA.

## Configure, build, test

```sh
cmake --preset dev-ninja
cmake --build --preset dev-ninja
ctest --preset dev-ninja

cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja

cmake --preset clang-time-trace
cmake --build --preset clang-time-trace
ctest --preset clang-time-trace

cmake --preset clang-tidy-ninja
cmake --build --preset clang-tidy-ninja
ctest --preset clang-tidy-ninja

cmake --preset release-clang-ninja
cmake --build --preset release-clang-ninja
ctest --preset release-clang-ninja

cmake --preset release-ninja
cmake --build --preset release-ninja
ctest --preset release-ninja

cmake --preset package-dev-ninja
cmake --build --preset package-dev-ninja
ctest --preset package-dev-ninja

cmake --preset package-release-clang-ninja
cmake --build --preset package-release-clang-ninja
ctest --preset package-release-clang-ninja
cmake --build --preset package-release-clang-ninja --target package
```

Benchmarks and package smoke scaffolding:

```sh
cmake --preset bench-dev-ninja
cmake --build --preset bench-dev-ninja
ctest --preset bench-dev-ninja -R '^pb_bench_' --output-on-failure

cmake --build --preset package-dev-ninja --target package
```

## CLI: `pb_cli`

The `pb_cli` tool is built into every preset and exposes graph export and
feature-gate introspection for the built-in example pipelines:

```sh
pb_cli list                                       # list example pipelines
pb_cli describe order-linear --format=dot          # render DOT to stdout
pb_cli describe order-branch --format=json         # render JSON to stdout
pb_cli describe order-fan-in --format=json \
       --out=order_fan_in.json                     # write to file
pb_cli describe order-branch --format=text         # compact text (compiler-error friendly)
pb_cli features                                    # print C++ feature matrix
```

Available built-in pipelines: `order-linear`, `order-branch`, `order-fan-in`.
The DOT/JSON/text output now follows the **stable `pb.core.graph.v1`
contract** — see [docs/export-schema-v1.md](docs/export-schema-v1.md) for
the formal spec, the migration policy, and the byte-equal regression tests
that lock the schema in place.

## Production-grade extras

### Engine error-policy wrappers
Stack any of these around a `pb::compile<P>(...)` result to swap the
error model without changing the underlying pipeline definition.  Each
wrapper preserves `try_run()` for callers that still want `result<>`,
and exposes `underlying()` for observer/descriptor access.

- `pb::with_throw_on_error(engine)` — converts any failure into a thrown
  `pb::pipeline_exception` on `run()`. The exception carries the full
  `pb::runtime::error` diagnostic via `ex.diagnostic()`.
- `pb::with_terminate_on_error(engine)` — calls `std::terminate()` on
  failure. Suitable for embedded / hard-real-time pipelines where any
  error is unrecoverable.
- `pb::with_ignore_errors(engine, fallback)` — substitutes a
  user-supplied fallback value on failure. The fallback is mutable via
  `set_fallback`.
- `pb::with_propagate_exceptions(engine)` — rethrows stage *exceptions*
  as `pb::pipeline_exception` while leaving declared `result<>` failures
  on the value path. Useful when you want a single `catch` to handle
  thrown bugs but still inspect business failures as values.
- `pb::with_verbose_diagnostics(engine, &stream)` — auto-attaches an
  observer that logs every stage transition to a `std::ostream` using
  the stable `[pb.verbose] <event> stage=<key>` line schema.

See [include/pb/runtime/error_policy.hpp](include/pb/runtime/error_policy.hpp).

### Fan-in clone / projection
- `pb::shared_view<T>` — copyable view that lets owned non-copyable
  values flow through fan-in branches via shared_ptr semantics.
- `pb::projected<From, Projection, Stage>` — wrap any stage so it
  accepts an upstream `From` type and applies `Projection{}(source)`
  before forwarding to `Stage`. Lets one stage participate in pipelines
  over different surrounding payload types without duplication.
- `pb::fan_in_uses_copy_v<Node>` / `pb::fan_in_uses_borrow_v<Node>` —
  compile-time introspection of the fan-in clone strategy.

See [include/pb/runtime/clone.hpp](include/pb/runtime/clone.hpp).

### Typed C++26 feature gates
- `pb::features::has_cpp26 / has_reflection / has_contracts /
  has_pack_indexing / has_std_expected / has_deducing_this` for
  `if constexpr` and concept gating in user code.
- Every `PB_HAS_*` macro respects user pre-defines so downstream builds
  can force-disable a feature to test their fallback path.
- `pb::reflect_stage<T>` is declared on every compiler but only emits a
  working reflection-driven adapter when `PB_HAS_REFLECTION == 1`. On
  C++20 baselines, instantiating it produces a clear `static_assert`
  pointing at the gate.

See [docs/cpp26-feature-gates.md](docs/cpp26-feature-gates.md) and
[include/pb/adapt/reflect.hpp](include/pb/adapt/reflect.hpp).
