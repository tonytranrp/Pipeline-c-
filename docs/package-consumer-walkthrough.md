# Package Consumer Walkthrough

This walkthrough explains the current downstream package contract for `pipebuilder`: how a consumer discovers the installed package with CMake, which targets are meant to be linked, and what the repository's package-smoke lane actually proves today.

## Intended downstream entry point

A downstream CMake consumer starts with the installed package config:

```cmake
find_package(pipebuilder CONFIG REQUIRED)
```

After `find_package`, the installed package is expected to expose three imported targets:

- `pb::core` — the core compile-time validation surface.
- `pb::runtime` — the runtime support target.
- `pb::pipeline` — the documented compatibility target for consumers that want the combined surface.

`pb::pipeline` currently forwards to `pb::runtime`, so most consumers can link that one target and get the public pipeline API plus runtime support.

## Recommended linking patterns

### Default application path

For a consumer that wants to build and run pipelines, prefer the compatibility target:

```cmake
add_executable(my_app main.cpp)
target_compile_features(my_app PRIVATE cxx_std_20)
target_link_libraries(my_app PRIVATE pb::pipeline)
```

This is the most stable documented path because it matches the public examples and the installed-package smoke coverage.

### Split-target paths

The installed package also exports the lower-level targets directly:

```cmake
add_executable(core_only_smoke core_only.cpp)
target_compile_features(core_only_smoke PRIVATE cxx_std_20)
target_link_libraries(core_only_smoke PRIVATE pb::core)

add_executable(runtime_only_smoke runtime_only.cpp)
target_compile_features(runtime_only_smoke PRIVATE cxx_std_20)
target_link_libraries(runtime_only_smoke PRIVATE pb::runtime)
```

Use these direct targets only when the consumer intentionally wants the narrower split surface. `pb::core` is suitable for compile-time validation/introspection-only code, while `pb::runtime` is suitable when the consumer needs runtime error or executor support explicitly.

## Minimal downstream example

A basic installed-package consumer can look like this:

```cpp
#include <pb/pipeline.hpp>

struct Increment {
  using input_type = int;
  using output_type = int;

  static constexpr auto stage_name() noexcept { return "increment"; }

  int operator()(int value) const { return value + 1; }
};

int main() {
  using Pipeline = pb::from<int>::then<Increment>::to<int>;
  static_assert(pb::ValidPipeline<Pipeline>);

  auto engine = pb::compile<Pipeline>(pb::runtime::sequential{});
  return engine.run(41) == 42 ? 0 : 1;
}
```

That matches the current package-smoke consumer path the repository verifies in CI-style local checks.

## What the package-smoke lane is intended to prove

The current installed-package contract is exercised by `pb_package_config_smoke` in the `package-release-clang-ninja` lane.

The script currently performs or attempts all of these checks:

1. Installs the current build into a temporary prefix.
2. Configures a clean downstream consumer with `find_package(pipebuilder CONFIG REQUIRED)` and `CMAKE_PREFIX_PATH` pointing at that install prefix.
3. Verifies that `pb::core`, `pb::runtime`, and `pb::pipeline` are all defined as imported targets.
4. Builds separate downstream executables that link each target independently.
5. Runs the generated package-consumer executable path to prove the linked pipeline binary executes successfully.
6. Runs CPack with `TGZ` output.
7. Confirms that the generated archive contains at least:
   - `include/pb/pipeline.hpp`
   - `lib/cmake/pipebuilder/pipebuilderConfig.cmake`
   - `lib/cmake/pipebuilder/pipebuilderTargets.cmake`
   - `lib/libpb_runtime.a`

When this lane is green, it proves more than just CMake configure success: it covers install-tree discovery, target export visibility, consumer compilation, consumer execution, and a minimum archive-content contract.

## Commands to reproduce the current contract

Use the release package lane when you want fresh evidence for installed-package consumers:

```bash
cmake --preset package-release-clang-ninja
cmake --build --preset package-release-clang-ninja
ctest --preset package-release-clang-ninja --output-on-failure -R pb_package_config_smoke
cmake --build --preset package-release-clang-ninja --target package
```

If you want an isolated local rerun without reusing an existing build directory, configure a fresh package-release build directory and run the same test/build flow there.

As of the current repository state, a fresh rerun is a release blocker: the generated consumer project builds `pipebuilder_core_target_smoke`, `pipebuilder_runtime_target_smoke`, and `pipebuilder_pipeline_alias_smoke`, but the smoke script later searches for a non-existent `pipebuilder_package_smoke` executable. Treat that mismatch as a packaging verification bug to fix before claiming the package-smoke lane is green again.

## Expectations and current limitations

The current downstream package contract is intentionally narrow:

- `pb::pipeline` is the primary documented consumer target.
- The split targets are exported and smoke-tested, but they are still part of the MVP scaffold rather than a fully versioned long-term package surface.
- The smoke lane proves successful consumption against the local toolchain used to build the package; wider cross-compiler/package-manager validation is still a release gate.
- A fresh rerun at the current HEAD fails in the generated-consumer execution step because the smoke script looks for `pipebuilder_package_smoke` while the generated project builds target-specific executables instead.
- The current archive check proves the runtime static library and key config/header entries exist, not that every future optional component or roadmap feature is packaged.
- Optional backends, branch/join graphs, graph export, observer hooks, and richer runtime descriptor surfaces remain roadmap-only work, not installed-package guarantees.

For the current supported release boundary, treat this walkthrough together with `docs/production-readiness.md` and the package-smoke script as the source of truth.
