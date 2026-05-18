/// @file policy_dsl.cpp
/// @brief Compile-time verification of all pb::policy tag types and the with<Policy> builder alias.
///
/// Every static_assert here validates that policy types are well-formed, composable, and
/// zero-overhead — the compiler eliminates every tag at zero runtime cost.

#include <pb/pipeline.hpp>

#include <type_traits>

// ---------------------------------------------------------------------------
// 1. Every policy tag is a complete, instantiable type
// ---------------------------------------------------------------------------
namespace policy_size_checks {

// errors
static_assert(sizeof(pb::policy::errors::expected) > 0);
static_assert(sizeof(pb::policy::errors::terminate) > 0);
static_assert(sizeof(pb::policy::errors::ignore) > 0);

// exceptions
static_assert(sizeof(pb::policy::exceptions::propagate) > 0);
static_assert(sizeof(pb::policy::exceptions::catch_all) > 0);

// memory
static_assert(sizeof(pb::policy::memory::copy) > 0);
static_assert(sizeof(pb::policy::memory::move) > 0);
static_assert(sizeof(pb::policy::memory::borrow) > 0);

// storage
static_assert(sizeof(pb::policy::storage::per_run) > 0);
static_assert(sizeof(pb::policy::storage::persistent) > 0);
static_assert(sizeof(pb::policy::storage::thread_local_storage) > 0);

// contracts
static_assert(sizeof(pb::policy::contracts::strict) > 0);
static_assert(sizeof(pb::policy::contracts::relaxed) > 0);

// diagnostics
static_assert(sizeof(pb::policy::diagnostics::silent) > 0);
static_assert(sizeof(pb::policy::diagnostics::normal) > 0);
static_assert(sizeof(pb::policy::diagnostics::verbose) > 0);

} // namespace policy_size_checks

// ---------------------------------------------------------------------------
// 2. Policy types are copyable, movable, and default-constructible
// ---------------------------------------------------------------------------
namespace policy_triviality_checks {

template <class T>
constexpr bool is_regular_tag =
    std::is_default_constructible_v<T> &&
    std::is_copy_constructible_v<T> &&
    std::is_move_constructible_v<T> &&
    std::is_copy_assignable_v<T> &&
    std::is_move_assignable_v<T> &&
    std::is_destructible_v<T>;

static_assert(is_regular_tag<pb::policy::errors::expected>);
static_assert(is_regular_tag<pb::policy::errors::terminate>);
static_assert(is_regular_tag<pb::policy::errors::ignore>);
static_assert(is_regular_tag<pb::policy::exceptions::propagate>);
static_assert(is_regular_tag<pb::policy::exceptions::catch_all>);
static_assert(is_regular_tag<pb::policy::memory::copy>);
static_assert(is_regular_tag<pb::policy::memory::move>);
static_assert(is_regular_tag<pb::policy::memory::borrow>);
static_assert(is_regular_tag<pb::policy::storage::per_run>);
static_assert(is_regular_tag<pb::policy::storage::persistent>);
static_assert(is_regular_tag<pb::policy::storage::thread_local_storage>);
static_assert(is_regular_tag<pb::policy::contracts::strict>);
static_assert(is_regular_tag<pb::policy::contracts::relaxed>);
static_assert(is_regular_tag<pb::policy::diagnostics::silent>);
static_assert(is_regular_tag<pb::policy::diagnostics::normal>);
static_assert(is_regular_tag<pb::policy::diagnostics::verbose>);

} // namespace policy_triviality_checks

// ---------------------------------------------------------------------------
// 3. Policy types are distinct — no accidental aliasing
// ---------------------------------------------------------------------------
namespace policy_distinctness_checks {

template <class T, class U>
concept distinct = !std::same_as<T, U>;

static_assert(distinct<pb::policy::errors::expected, pb::policy::errors::terminate>);
static_assert(distinct<pb::policy::errors::expected, pb::policy::errors::ignore>);
static_assert(distinct<pb::policy::errors::terminate, pb::policy::errors::ignore>);
static_assert(distinct<pb::policy::exceptions::propagate, pb::policy::exceptions::catch_all>);
static_assert(distinct<pb::policy::memory::copy, pb::policy::memory::move>);
static_assert(distinct<pb::policy::memory::copy, pb::policy::memory::borrow>);
static_assert(distinct<pb::policy::memory::move, pb::policy::memory::borrow>);
static_assert(distinct<pb::policy::storage::per_run, pb::policy::storage::persistent>);
static_assert(distinct<pb::policy::storage::per_run, pb::policy::storage::thread_local_storage>);
static_assert(distinct<pb::policy::storage::persistent, pb::policy::storage::thread_local_storage>);
static_assert(distinct<pb::policy::contracts::strict, pb::policy::contracts::relaxed>);
static_assert(distinct<pb::policy::diagnostics::silent, pb::policy::diagnostics::normal>);
static_assert(distinct<pb::policy::diagnostics::silent, pb::policy::diagnostics::verbose>);
static_assert(distinct<pb::policy::diagnostics::normal, pb::policy::diagnostics::verbose>);

} // namespace policy_distinctness_checks

// ---------------------------------------------------------------------------
// 4. with<Policy> builder chain — policies compose without altering pipeline type identity
// ---------------------------------------------------------------------------
namespace policy_with_builder_checks {

struct Raw { int value{}; };
struct Parsed { int value{}; };
struct Done { int value{}; };

struct Parse {
  using input_type = Raw;
  using output_type = Parsed;
  static constexpr auto stage_name() noexcept { return "parse"; }
  constexpr auto operator()(Raw r) const noexcept -> Parsed { return {r.value + 1}; }
};

struct Finish {
  using input_type = Parsed;
  using output_type = Done;
  static constexpr auto stage_name() noexcept { return "finish"; }
  constexpr auto operator()(Parsed p) const noexcept -> Done { return {p.value * 2}; }
};

// Baseline pipeline (no policies)
using Baseline = pb::from<Raw>::then<Parse>::then<Finish>::to<Done>;

// Pipeline with a single policy — type identity is unchanged
using WithOnePolicy =
    pb::from<Raw>::then<Parse>::template with<pb::policy::storage::persistent>::then<Finish>::to<Done>;
static_assert(std::same_as<Baseline, WithOnePolicy>,
              "with<Policy> must be a pass-through that preserves pipeline type identity");

// Pipeline with multiple policies
using WithManyPolicies =
    pb::from<Raw>::then<Parse>::template
        with<pb::policy::storage::persistent,
             pb::policy::contracts::strict,
             pb::policy::diagnostics::verbose>::then<Finish>::to<Done>;
static_assert(std::same_as<Baseline, WithManyPolicies>,
              "with<Policy...> with multiple policies must preserve pipeline type identity");

// Pipeline with zero policies (empty pack)
using WithZeroPolicies =
    pb::from<Raw>::then<Parse>::template with<>::then<Finish>::to<Done>;
static_assert(std::same_as<Baseline, WithZeroPolicies>,
              "with<> with empty pack must preserve pipeline type identity");

// Verify the pipeline is still valid and runnable
static_assert(pb::valid<WithManyPolicies>);
static_assert(pb::ValidPipeline<WithManyPolicies>);
static_assert(pb::is_pipeline_v<WithManyPolicies>);
static_assert(std::same_as<pb::pipeline_input_t<WithManyPolicies>, Raw>);
static_assert(std::same_as<pb::pipeline_output_t<WithManyPolicies>, Done>);
static_assert(pb::pipeline_size_v<WithManyPolicies> == 2);

} // namespace policy_with_builder_checks

// ---------------------------------------------------------------------------
// 5. Policy types are usable as constexpr values (zero-cost tag dispatch)
// ---------------------------------------------------------------------------
namespace policy_constexpr_usage {

constexpr auto err_policy = pb::policy::errors::expected{};
constexpr auto mem_policy = pb::policy::memory::move{};
constexpr auto store_policy = pb::policy::storage::persistent{};
constexpr auto contract_policy = pb::policy::contracts::strict{};
constexpr auto diag_policy = pb::policy::diagnostics::verbose{};

static_assert(sizeof(err_policy) == 1, "Empty tag type should occupy 1 byte");
static_assert(sizeof(mem_policy) == 1);
static_assert(sizeof(store_policy) == 1);
static_assert(sizeof(contract_policy) == 1);
static_assert(sizeof(diag_policy) == 1);

// Policy tags compose into tuples with zero storage overhead beyond alignment
using PolicyTuple = std::tuple<
    pb::policy::errors::expected,
    pb::policy::memory::move,
    pb::policy::storage::persistent,
    pb::policy::contracts::strict,
    pb::policy::diagnostics::verbose>;

// An empty tuple of empty types can be zero-size with EBO on compliant compilers
// (MSVC may not apply EBO; verify reasonable upper bound instead)
static_assert(sizeof(PolicyTuple) <= sizeof(int) * 8,
              "Policy tuple should have reasonable size with empty types");

} // namespace policy_constexpr_usage

int main() { return 0; }
