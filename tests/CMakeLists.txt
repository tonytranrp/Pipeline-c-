add_executable(pb_smoke_compile_pass compile_pass/smoke_pipeline.cpp)
target_link_libraries(pb_smoke_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_smoke_compile_pass PRIVATE)
add_test(NAME pb_smoke_compile_pass COMMAND pb_smoke_compile_pass)


add_executable(pb_describe_compile_pass compile_pass/describe_pipeline.cpp)
target_link_libraries(pb_describe_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_describe_compile_pass PRIVATE)
add_test(NAME pb_describe_compile_pass COMMAND pb_describe_compile_pass)

add_executable(pb_runtime_smoke runtime/sequential_smoke.cpp)
target_link_libraries(pb_runtime_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_smoke PRIVATE)
add_test(NAME pb_runtime_smoke COMMAND pb_runtime_smoke)

add_executable(pb_runtime_result_smoke runtime/result_smoke.cpp)
target_link_libraries(pb_runtime_result_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_result_smoke PRIVATE)
add_test(NAME pb_runtime_result_smoke COMMAND pb_runtime_result_smoke)

add_executable(pb_runtime_sequential_result_smoke runtime/sequential_result_smoke.cpp)
target_link_libraries(pb_runtime_sequential_result_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_result_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_result_smoke COMMAND pb_runtime_sequential_result_smoke)

add_executable(pb_runtime_zero_stage_smoke runtime/zero_stage_smoke.cpp)
target_link_libraries(pb_runtime_zero_stage_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_zero_stage_smoke PRIVATE)
add_test(NAME pb_runtime_zero_stage_smoke COMMAND pb_runtime_zero_stage_smoke)

add_test(NAME pb_compile_fail_edge_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)

add_test(NAME pb_compile_fail_sink_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/sink_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ sink\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
