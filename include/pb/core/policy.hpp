#pragma once

/// @file
/// @brief Baseline policy DSL — compile-time tag types for pipeline behaviour configuration.
///
/// Every policy type is an empty tag struct.  Users compose them as template arguments
/// (e.g. pb::with<policy::storage::persistent, policy::contracts::strict>) to signal
/// compile-time behavioural choices without runtime overhead.  The compiler eliminates
/// every tag — they are zero-size, zero-cost, and never appear in generated code.

namespace pb::policy {

// ---------------------------------------------------------------------------
// Error-handling policies
// ---------------------------------------------------------------------------
namespace errors {

/// Use std::expected / expected-like error handling.
/// Stages that return result<T, E> propagate errors through the pipeline.
struct expected {};

/// Terminate on stage failure (std::terminate / std::abort).
/// Suitable for pipelines where any failure is unrecoverable.
struct terminate {};

/// Ignore errors; continue pipeline execution regardless.
/// Use with caution — only for best-effort / fire-and-forget workloads.
struct ignore {};

} // namespace errors

// ---------------------------------------------------------------------------
// Exception policies
// ---------------------------------------------------------------------------
namespace exceptions {

/// Let exceptions propagate naturally through the call stack.
/// The caller is responsible for catching and handling them.
struct propagate {};

/// Catch all exceptions at stage boundaries, convert them into
/// the pipeline's error type (e.g. result<T, E>).
struct catch_all {};

} // namespace exceptions

// ---------------------------------------------------------------------------
// Copy / move / borrow policies (value-category semantics)
// ---------------------------------------------------------------------------
namespace memory {

/// Copy inputs between stages.
/// Safe, but may incur heap allocations for large objects.
struct copy {};

/// Move inputs between stages (std::move semantics).
/// Zero-copy for movable types; leaves source in valid-but-unspecified state.
struct move {};

/// Pass by const reference (borrow semantics).
/// Zero-copy, zero-overhead; the caller retains ownership.
struct borrow {};

} // namespace memory

// ---------------------------------------------------------------------------
// State-storage policies (pipeline engine lifecycle)
// ---------------------------------------------------------------------------
namespace storage {

/// Default-construct stages on every pipeline run.
/// Stateless stages have zero runtime cost with this policy.
struct per_run {};

/// Store stage instances inside the engine, reusing them across runs.
/// Enables stages to carry mutable state between invocations.
struct persistent {};

/// Thread-local storage for stage instances.
/// Each thread gets its own copy; useful with parallel backends.
struct thread_local_storage {};

} // namespace storage

// ---------------------------------------------------------------------------
// Assertion / contract policies
// ---------------------------------------------------------------------------
namespace contracts {

/// Static assertions and compile-time enforcement.
/// Every type constraint is verified at compile-time with static_assert.
struct strict {};

/// Runtime checks only (assert / custom verification).
/// Compile-time checks are relaxed; validation deferred to runtime.
struct relaxed {};

} // namespace contracts

// ---------------------------------------------------------------------------
// Diagnostic verbosity policies
// ---------------------------------------------------------------------------
namespace diagnostics {

/// No diagnostics emitted — silence is golden.
struct silent {};

/// Standard diagnostics — enough to understand what's happening.
struct normal {};

/// Detailed diagnostics — every stage transition, timing, and value dump.
struct verbose {};

} // namespace diagnostics

} // namespace pb::policy
