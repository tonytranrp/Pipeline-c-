# Pipeline Builder C++ Docs

## Purpose

This repository is bootstrapping a typed C++20 pipeline builder. This docs page is the entry point for build, examples, tests, and benchmark scaffolding.

## Current scaffold

- `tests/compile_pass/` for positive compile checks
- `tests/compile_fail/` for negative diagnostic checks
- `tests/runtime/` for runtime smoke checks
- `bench/compile_time/` for public-header include and 50-stage chain compile checks
- `bench/runtime/` for a 5-stage chain smoke target
