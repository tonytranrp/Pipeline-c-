#!/usr/bin/env bash
set -euo pipefail

preset="${1:-clang-time-trace}"
shift || true

targets=("${@:-}")
if [ "${#targets[@]}" -eq 0 ] || [ -z "${targets[0]}" ]; then
  targets=(
    pb_bench_include_pipeline
    pb_bench_compile_chain_5
    pb_bench_compile_chain_50
  )
fi

echo "# pipebuilder compile-time baseline collection"
echo "preset=${preset}"
echo "targets=${targets[*]}"
echo "note=records local timing commands only; no threshold or budget is implied"

cmake --preset "${preset}" -DPB_BUILD_BENCHMARKS=ON

for target in "${targets[@]}"; do
  echo "## build target=${target}"
  cmake -E time cmake --build --preset "${preset}" --target "${target}"
done

if [ "${preset}" = "clang-time-trace" ]; then
  echo "## clang time-trace artifacts"
  find "build/${preset}" -name '*.json' -path '*CMakeFiles*' -print | sort
fi
