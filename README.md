# Pipeline-c++

Compile-time validated C++20 pipeline builder foundation.

This first base follows `research/pipeline_builder_cpp_research_plan.md`:

- target-based CMake project (`pb::core`, `pb::runtime`, `pb::pipeline`)
- LLVM/Clang-friendly presets and `compile_commands.json`
- public `.hpp` headers under `include/pb/`
- implementation files under `src/`
- examples, tests, docs, and benchmark smoke folders
- MVP `pb::from<T>::then<S>::to<U>` typed linear pipeline validation
- sequential runtime executor and free/function-object adapters

## Configure, build, test

```sh
cmake --preset clang-dev-ninja
cmake --build --preset clang-dev-ninja
ctest --preset clang-dev-ninja
```

If `clang++` is not on `PATH`, use the generic preset:

```sh
cmake --preset dev-ninja
cmake --build --preset dev-ninja
ctest --preset dev-ninja
```
