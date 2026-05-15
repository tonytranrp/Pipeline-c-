add_executable(pb_smoke_compile_pass compile_pass/smoke_pipeline.cpp)
target_link_libraries(pb_smoke_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_smoke_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_smoke_compile_pass)
add_test(NAME pb_smoke_compile_pass COMMAND pb_smoke_compile_pass)
set_tests_properties(pb_smoke_compile_pass PROPERTIES LABELS "compile-pass;smoke")


add_executable(pb_describe_compile_pass compile_pass/describe_pipeline.cpp)
target_link_libraries(pb_describe_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_describe_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_describe_compile_pass)
add_test(NAME pb_describe_compile_pass COMMAND pb_describe_compile_pass)
set_tests_properties(pb_describe_compile_pass PROPERTIES LABELS "compile-pass;metadata")

add_executable(pb_public_headers_compile_pass
  compile_pass/public_headers.cpp
  compile_pass/public_headers/adapt_fn.cpp
  compile_pass/public_headers/core_concepts.cpp
  compile_pass/public_headers/core_describe.cpp
  compile_pass/public_headers/core_fixed_string.cpp
  compile_pass/public_headers/core_meta.cpp
  compile_pass/public_headers/core_pipeline_state.cpp
  compile_pass/public_headers/core_stage_traits.cpp
  compile_pass/public_headers/core_validate.cpp
  compile_pass/public_headers/pipeline.cpp
  compile_pass/public_headers/runtime_error.cpp
  compile_pass/public_headers/runtime_result.cpp
  compile_pass/public_headers/runtime_sequential.cpp
)
target_link_libraries(pb_public_headers_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_public_headers_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_public_headers_compile_pass)
add_test(NAME pb_public_headers_compile_pass COMMAND pb_public_headers_compile_pass)
set_tests_properties(pb_public_headers_compile_pass PROPERTIES LABELS "compile-pass;headers")

add_executable(pb_runtime_smoke runtime/sequential_smoke.cpp)
target_link_libraries(pb_runtime_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_smoke)
add_test(NAME pb_runtime_smoke COMMAND pb_runtime_smoke)
set_tests_properties(pb_runtime_smoke PROPERTIES LABELS "runtime;smoke")

add_executable(pb_runtime_result_smoke runtime/result_smoke.cpp)
target_link_libraries(pb_runtime_result_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_result_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_result_smoke)
add_test(NAME pb_runtime_result_smoke COMMAND pb_runtime_result_smoke)
set_tests_properties(pb_runtime_result_smoke PROPERTIES LABELS "runtime;result")

add_executable(pb_runtime_sequential_result_smoke runtime/sequential_result_smoke.cpp)
target_link_libraries(pb_runtime_sequential_result_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_result_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_result_smoke)
add_test(NAME pb_runtime_sequential_result_smoke COMMAND pb_runtime_sequential_result_smoke)
set_tests_properties(pb_runtime_sequential_result_smoke PROPERTIES LABELS "runtime;result")

add_executable(pb_runtime_zero_stage_smoke runtime/zero_stage_smoke.cpp)
target_link_libraries(pb_runtime_zero_stage_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_zero_stage_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_zero_stage_smoke)
add_test(NAME pb_runtime_zero_stage_smoke COMMAND pb_runtime_zero_stage_smoke)
set_tests_properties(pb_runtime_zero_stage_smoke PROPERTIES LABELS "runtime;smoke")

add_executable(pb_runtime_sequential_expected_like_smoke runtime/sequential_expected_like_smoke.cpp)
target_link_libraries(pb_runtime_sequential_expected_like_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_expected_like_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_expected_like_smoke)
add_test(NAME pb_runtime_sequential_expected_like_smoke COMMAND pb_runtime_sequential_expected_like_smoke)
set_tests_properties(pb_runtime_sequential_expected_like_smoke PROPERTIES LABELS "runtime;result")

add_test(NAME pb_compile_fail_edge_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_edge_mismatch PROPERTIES LABELS "compile-fail;diagnostics")


add_test(NAME pb_compile_fail_invalid_stage
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/invalid_stage.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ is\ invalid
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_invalid_stage PROPERTIES LABELS "compile-fail;diagnostics")

add_test(NAME pb_compile_fail_missing_input_stage
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/missing_input_stage.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ is\ invalid
    -DEXPECTED_DIAGNOSTICS=define\ input_type\ and\ output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_missing_input_stage PROPERTIES LABELS "compile-fail;diagnostics")

add_test(NAME pb_compile_fail_sink_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/sink_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ sink\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_sink_mismatch PROPERTIES LABELS "compile-fail;diagnostics")
