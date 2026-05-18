#pragma once

// ─────────────────────────────────────────────────────────────
// C++26 (and beyond) feature‑detection macros
//
// These macros allow the library to opt into future language
// facilities when available while remaining compatible with
// C++20 baseline compilers.
// ─────────────────────────────────────────────────────────────

// ── C++26 core language ─────────────────────────────────────

#if __cplusplus >= 202400L
  #define PB_HAS_CPP26 1
#else
  #define PB_HAS_CPP26 0
#endif

// ── static reflection (P2996 / P1240) ───────────────────────

#if PB_HAS_CPP26 && defined(__cpp_reflection)
  #define PB_HAS_REFLECTION 1
#else
  #define PB_HAS_REFLECTION 0
#endif

// ── contracts (P2900) ───────────────────────────────────────

#if PB_HAS_CPP26 && defined(__cpp_contracts)
  #define PB_HAS_CONTRACTS 1
#else
  #define PB_HAS_CONTRACTS 0
#endif

// ── pack indexing (P2662) ────────────────────────────────────
// This feature may also land in C++23 with sufficient compiler
// support, so the guard is independent of the PB_HAS_CPP26 gate.

#if defined(__cpp_pack_indexing)
  #define PB_HAS_PACK_INDEXING 1
#else
  #define PB_HAS_PACK_INDEXING 0
#endif

// ── std::expected (C++23) ────────────────────────────────────
// While not C++26, this is the primary "modern result type" and
// is worth a dedicated feature gate for conditional compilation.

#if defined(__cpp_lib_expected) && __cpp_lib_expected >= 202202L
  #define PB_HAS_STD_EXPECTED 1
#else
  #define PB_HAS_STD_EXPECTED 0
#endif

// ── deducing this (C++23, P0847) ─────────────────────────────

#if defined(__cpp_explicit_this_parameter)
  #define PB_HAS_DEDUCING_THIS 1
#else
  #define PB_HAS_DEDUCING_THIS 0
#endif

// ── Compile‑time validation bundle ───────────────────────────
// These static_asserts fire at library inclusion time to
// surface misconfigured feature‑detection scenarios.

static_assert((PB_HAS_CPP26 == 0) || (PB_HAS_CPP26 == 1),
              "PB_HAS_CPP26 must be 0 or 1");

static_assert((PB_HAS_REFLECTION == 0) || (PB_HAS_REFLECTION == 1),
              "PB_HAS_REFLECTION must be 0 or 1");

static_assert((PB_HAS_CONTRACTS == 0) || (PB_HAS_CONTRACTS == 1),
              "PB_HAS_CONTRACTS must be 0 or 1");

static_assert((PB_HAS_PACK_INDEXING == 0) || (PB_HAS_PACK_INDEXING == 1),
              "PB_HAS_PACK_INDEXING must be 0 or 1");

static_assert((PB_HAS_STD_EXPECTED == 0) || (PB_HAS_STD_EXPECTED == 1),
              "PB_HAS_STD_EXPECTED must be 0 or 1");

static_assert((PB_HAS_DEDUCING_THIS == 0) || (PB_HAS_DEDUCING_THIS == 1),
              "PB_HAS_DEDUCING_THIS must be 0 or 1");

// PB_HAS_CPP26 implies the compiler claims C++26 or later.
// The other feature gates are independently gated on the
// corresponding feature‑test macros and may be 0 even when
// PB_HAS_CPP26 is 1 (e.g. a C++26 compiler that hasn't shipped
// reflection yet).
