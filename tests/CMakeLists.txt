add_executable(pb_smoke_compile_pass compile_pass/smoke_pipeline.cpp)
add_test(NAME pb_smoke_compile_pass COMMAND pb_smoke_compile_pass)

add_executable(pb_runtime_smoke runtime/sequential_smoke.cpp)
add_test(NAME pb_runtime_smoke COMMAND pb_runtime_smoke)

add_test(NAME pb_compile_fail_edge_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
