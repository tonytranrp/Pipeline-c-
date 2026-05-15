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
- [Graph Export Roadmap / Status](docs/graph-export-roadmap.md) explains why graph export is still roadmap-only and what must land before docs can present it as supported.

For package consumers, the release-readiness path is the `package-release-clang-ninja` configure, build, CTest, and package target sequence below. That CTest run includes `pb_package_config_smoke`, which is intended to install the package into a temporary prefix, verify that `find_package(pipebuilder CONFIG REQUIRED)` exposes `pb::core`, `pb::runtime`, and the `pb::pipeline` compatibility target, and build separate downstream consumers against each target. The current package-consumer walkthrough records a known smoke-script blocker around the generated executable name; do not treat the package lane as release-green until that blocker is fixed and the preset passes freshly.

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
