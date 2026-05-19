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
