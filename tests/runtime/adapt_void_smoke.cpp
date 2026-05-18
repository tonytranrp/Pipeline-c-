#include <pb/pipeline.hpp>

#include <cassert>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Test types
// ---------------------------------------------------------------------------
struct Input {
  int value{};
};

struct Processed {
  int value{};
};

struct Output {
  int value{};
};

// ---------------------------------------------------------------------------
// Side-effect recording functor for void adapter
// ---------------------------------------------------------------------------
struct SideEffectLogger {
  std::vector<std::string>* log = nullptr;
  std::string label;

  SideEffectLogger() = default;
  SideEffectLogger(std::vector<std::string>* l, std::string lbl)
      : log(l), label(std::move(lbl)) {}

  void operator()(Processed p) const {
    if (log) log->push_back(label + ":" + std::to_string(p.value));
  }
};

struct SideEffectNoexceptLogger {
  std::vector<std::string>* log = nullptr;
  std::string label;

  SideEffectNoexceptLogger() = default;
  SideEffectNoexceptLogger(std::vector<std::string>* l, std::string lbl)
      : log(l), label(std::move(lbl)) {}

  void operator()(Processed p) const noexcept {
    if (log) log->push_back(label + ":noexcept:" + std::to_string(p.value));
  }
};

// ---------------------------------------------------------------------------
// Default-constructible functor for void adapter
// ---------------------------------------------------------------------------
struct CountingLogger {
  int* counter = nullptr;

  CountingLogger() = default;
  explicit CountingLogger(int* c) : counter(c) {}

  void operator()(Processed p) const {
    if (counter) *counter += 1;
  }
};

// ---------------------------------------------------------------------------
// Pipeline stages
// ---------------------------------------------------------------------------
struct ProcessStage {
  using input_type = Input;
  using output_type = Processed;

  static constexpr std::string_view stage_name() noexcept { return "process"; }

  Processed operator()(Input input) const { return {input.value + 1}; }
};

struct FinalizeStage {
  using input_type = Processed;
  using output_type = Output;

  static constexpr std::string_view stage_name() noexcept { return "finalize"; }

  Output operator()(Processed input) const { return {input.value * 2}; }
};

// ---------------------------------------------------------------------------
// Void adapter type aliases
// ---------------------------------------------------------------------------
using FnVoidAdapter = pb::void_adapter<SideEffectLogger, Processed>;
using NoexceptFnVoidAdapter = pb::void_adapter<SideEffectNoexceptLogger, Processed>;
using FunctorVoidAdapter = pb::void_adapter<CountingLogger, Processed>;

// ---------------------------------------------------------------------------
// Pipeline type aliases
// ---------------------------------------------------------------------------
using FnVoidPipeline =
    pb::from<Input>::then<ProcessStage>::then<FnVoidAdapter>::then<FinalizeStage>::to<Output>;

using NoexceptFnVoidPipeline =
    pb::from<Input>::then<ProcessStage>::then<NoexceptFnVoidAdapter>::then<FinalizeStage>::to<Output>;

using FunctorVoidPipeline =
    pb::from<Input>::then<ProcessStage>::then<FunctorVoidAdapter>::then<FinalizeStage>::to<Output>;

// Compile-time assertions
static_assert(pb::Stage<FnVoidAdapter>);
static_assert(pb::Stage<NoexceptFnVoidAdapter>);
static_assert(pb::Stage<FunctorVoidAdapter>);
static_assert(pb::valid<FnVoidPipeline>);
static_assert(pb::valid<NoexceptFnVoidPipeline>);
static_assert(pb::valid<FunctorVoidPipeline>);
static_assert(pb::AdjacentStages<ProcessStage, FnVoidAdapter>);
static_assert(pb::AdjacentStages<FnVoidAdapter, FinalizeStage>);
static_assert(pb::AdjacentStages<ProcessStage, NoexceptFnVoidAdapter>);
static_assert(pb::AdjacentStages<NoexceptFnVoidAdapter, FinalizeStage>);

// ---------------------------------------------------------------------------
// Runtime tests
// ---------------------------------------------------------------------------
int main() {
  // Test 1: void_adapter pass-through with stateful functor
  {
    std::vector<std::string> log;
    SideEffectLogger logger{&log, "test1"};
    auto adapter = pb::void_adapter<SideEffectLogger, Processed>{logger};
    Processed input{42};
    auto result = adapter(input);
    assert(result.value == 42); // pass-through
    assert(log.size() == 1);
    assert(log[0] == "test1:42");
  }

  // Test 2: noexcept void_adapter pass-through
  {
    std::vector<std::string> log;
    SideEffectNoexceptLogger logger{&log, "test2"};
    auto adapter = pb::void_adapter<SideEffectNoexceptLogger, Processed>{logger};
    Processed input{17};
    auto result = adapter(input);
    assert(result.value == 17); // pass-through
    assert(log.size() == 1);
    assert(log[0] == "test2:noexcept:17");
  }

  // Test 3: Default-constructible functor void_adapter
  {
    int counter = 0;
    CountingLogger logger{&counter};
    auto adapter = pb::void_adapter<CountingLogger, Processed>{logger};
    Processed input{5};
    auto result = adapter(input);
    assert(result.value == 5);
    assert(counter == 1);
  }

  // Test 4: Pipeline with void adapter (construct_per_run — default-constructed logger)
  //         The default-constructed SideEffectLogger has null log pointer, so
  //         operator() is safely a no-op. The pipeline still runs correctly.
  {
    auto engine = pb::compile<FnVoidPipeline>(pb::runtime::sequential{});

    auto result = engine.run(Input{5});
    assert(result.value == 12); // (5+1)*2 = 12
  }

  // Test 5: Pipeline with noexcept void adapter
  {
    auto engine = pb::compile<NoexceptFnVoidPipeline>(pb::runtime::sequential{});

    auto result = engine.run(Input{3});
    assert(result.value == 8); // (3+1)*2 = 8
  }

  // Test 6: Pipeline with functor void adapter (construct_per_run)
  {
    auto engine = pb::compile<FunctorVoidPipeline>(pb::runtime::sequential{});

    auto result = engine.run(Input{10});
    assert(result.value == 22); // (10+1)*2 = 22
    // Functor with null counter still passes through without crashing
  }

  // Test 7: Noexcept detection matches actual behavior
  {
    static_assert(pb::is_noexcept_stage_v<NoexceptFnVoidAdapter, Processed>,
                  "noexcept void adapter must be noexcept");
    static_assert(!pb::is_noexcept_stage_v<FnVoidAdapter, Processed>,
                  "non-noexcept void adapter must not be detected as noexcept");
  }

  // Test 8: Void adapter as final stage (pipeline ending with void adapter)
  {
    using VoidFinalPipeline = pb::from<Input>::then<ProcessStage>::then<FnVoidAdapter>::to<Processed>;

    auto engine = pb::compile<VoidFinalPipeline>(pb::runtime::sequential{});

    auto result = engine.run(Input{7});
    assert(result.value == 8); // 7+1=8
  }

  // Test 9: Multiple void adapters chained
  {
    using MultiVoidPipeline =
        pb::from<Input>::then<ProcessStage>::then<FnVoidAdapter>::then<FnVoidAdapter>::then<FinalizeStage>::to<Output>;

    auto engine = pb::compile<MultiVoidPipeline>(pb::runtime::sequential{});

    auto result = engine.run(Input{1});
    assert(result.value == 4); // (1+1)*2 = 4
  }

  // Test 10: void_adapter satisfies adapted_stage
  {
    static_assert(pb::adapted_stage<FnVoidAdapter>,
                  "void_adapter with functor must satisfy adapted_stage");
    static_assert(pb::adapted_stage<FunctorVoidAdapter>,
                  "void_adapter with CountingLogger must satisfy adapted_stage");
  }

  // Test 11: try_run through void adapter pipeline — try_run always returns result<T>
  {
    auto engine = pb::compile<FnVoidPipeline>(pb::runtime::sequential{});

    auto result = engine.try_run(Input{2});
    assert(result.has_value());
    assert(result.value().value == 6); // (2+1)*2 = 6
  }

  return 0;
}
