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

add_executable(pb_expected_like_result_ok compile_pass/expected_like_result_ok.cpp)
target_link_libraries(pb_expected_like_result_ok PRIVATE pb::pipeline)
pb_apply_project_options(pb_expected_like_result_ok PRIVATE)
pb_enable_pch_if_requested(pb_expected_like_result_ok)
add_test(NAME pb_expected_like_result_ok COMMAND pb_expected_like_result_ok)
set_tests_properties(pb_expected_like_result_ok PROPERTIES LABELS "compile-pass;result")

add_executable(pb_stage_info_compile_pass compile_pass/stage_info.cpp)
target_link_libraries(pb_stage_info_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_stage_info_compile_pass PRIVATE)
add_test(NAME pb_stage_info_compile_pass COMMAND pb_stage_info_compile_pass)
set_tests_properties(pb_stage_info_compile_pass PROPERTIES LABELS "compile-pass;metadata")

add_executable(pb_public_headers_compile_pass
  compile_pass/public_headers.cpp
  compile_pass/public_headers/adapt_fn.cpp
  compile_pass/public_headers/adapt_member.cpp
  compile_pass/public_headers/adaptors.cpp
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
  compile_pass/public_headers/runtime_observer.cpp
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

add_executable(pb_runtime_error_diagnostic_smoke runtime/error_diagnostic_smoke.cpp)
target_link_libraries(pb_runtime_error_diagnostic_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_error_diagnostic_smoke PRIVATE)
add_test(NAME pb_runtime_error_diagnostic_smoke COMMAND pb_runtime_error_diagnostic_smoke)
set_tests_properties(pb_runtime_error_diagnostic_smoke PROPERTIES LABELS "runtime;diagnostics")

add_executable(pb_runtime_sequential_try_run_smoke runtime/sequential_try_run_smoke.cpp)
target_link_libraries(pb_runtime_sequential_try_run_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_try_run_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_try_run_smoke COMMAND pb_runtime_sequential_try_run_smoke)
set_tests_properties(pb_runtime_sequential_try_run_smoke PROPERTIES LABELS "runtime;result")

add_executable(pb_runtime_sequential_try_run_custom_error_expected_like_smoke
  runtime/sequential_try_run_custom_error_expected_like_smoke.cpp)
target_link_libraries(pb_runtime_sequential_try_run_custom_error_expected_like_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_try_run_custom_error_expected_like_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_try_run_custom_error_expected_like_smoke
  COMMAND pb_runtime_sequential_try_run_custom_error_expected_like_smoke)
set_tests_properties(pb_runtime_sequential_try_run_custom_error_expected_like_smoke PROPERTIES LABELS "runtime;result")

add_executable(pb_runtime_sequential_result_stage_annotation_smoke runtime/sequential_result_stage_annotation_smoke.cpp)
target_link_libraries(pb_runtime_sequential_result_stage_annotation_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_result_stage_annotation_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_result_stage_annotation_smoke COMMAND pb_runtime_sequential_result_stage_annotation_smoke)
set_tests_properties(pb_runtime_sequential_result_stage_annotation_smoke PROPERTIES LABELS "runtime;result;diagnostics")

add_executable(pb_runtime_sequential_custom_error_observer_smoke runtime/sequential_custom_error_observer_smoke.cpp)
target_link_libraries(pb_runtime_sequential_custom_error_observer_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_custom_error_observer_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_custom_error_observer_smoke COMMAND pb_runtime_sequential_custom_error_observer_smoke)
set_tests_properties(pb_runtime_sequential_custom_error_observer_smoke PROPERTIES LABELS "runtime;result;observer;diagnostics")

add_executable(pb_runtime_sequential_run_exception_smoke runtime/sequential_run_exception_smoke.cpp)
target_link_libraries(pb_runtime_sequential_run_exception_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_run_exception_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_run_exception_smoke COMMAND pb_runtime_sequential_run_exception_smoke)
set_tests_properties(pb_runtime_sequential_run_exception_smoke PROPERTIES LABELS "runtime;result")

add_executable(pb_runtime_sequential_observer_smoke runtime/sequential_observer_smoke.cpp)
target_link_libraries(pb_runtime_sequential_observer_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_observer_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_observer_smoke COMMAND pb_runtime_sequential_observer_smoke)
set_tests_properties(pb_runtime_sequential_observer_smoke PROPERTIES LABELS "runtime;observer")

add_executable(pb_runtime_sequential_observer_try_run_exception_smoke runtime/sequential_observer_try_run_exception_smoke.cpp)
target_link_libraries(pb_runtime_sequential_observer_try_run_exception_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_observer_try_run_exception_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_observer_try_run_exception_smoke COMMAND pb_runtime_sequential_observer_try_run_exception_smoke)
set_tests_properties(pb_runtime_sequential_observer_try_run_exception_smoke PROPERTIES LABELS "runtime;observer;result")

add_executable(pb_runtime_sequential_observer_accessor_smoke runtime/sequential_observer_accessor_smoke.cpp)
target_link_libraries(pb_runtime_sequential_observer_accessor_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_observer_accessor_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_observer_accessor_smoke COMMAND pb_runtime_sequential_observer_accessor_smoke)
set_tests_properties(pb_runtime_sequential_observer_accessor_smoke PROPERTIES LABELS "runtime;observer")

add_executable(pb_runtime_sequential_adapters_smoke runtime/sequential_adapters_smoke.cpp)
target_link_libraries(pb_runtime_sequential_adapters_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_adapters_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_adapters_smoke COMMAND pb_runtime_sequential_adapters_smoke)
set_tests_properties(pb_runtime_sequential_adapters_smoke PROPERTIES LABELS "runtime;adapter")

add_test(NAME pb_compile_fail_edge_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -DEXPECTED_DIAGNOSTICS=pb::connectable_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_edge_mismatch PROPERTIES LABELS "compile-fail;diagnostics")

add_test(NAME pb_compile_fail_adapt_fn_callable_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_fn_callable_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ callable\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_fn_callable_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_member_callable_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_member_callable_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ callable\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_member_callable_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_functor_callable_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_functor_callable_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ callable\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_functor_callable_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_direct_member_callable_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_direct_member_callable_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ callable\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_direct_member_callable_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_fn_output_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_fn_output_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ output\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_fn_output_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_functor_output_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_functor_output_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ output\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_functor_output_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_direct_member_output_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_direct_member_output_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ output\ mismatch
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_direct_member_output_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_invalid_stage
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/invalid_stage.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ is\ invalid
    -DEXPECTED_DIAGNOSTICS=define\ input_type\ and\ output_type
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


add_test(NAME pb_compile_fail_result_error_type_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/result_error_type_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline stages that return result<T, E> must use a compatible error type
  -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_result_error_type_mismatch PROPERTIES LABELS "compile-fail;diagnostics")

add_test(NAME pb_compile_fail_pipeline_stage_key_index_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_key_index_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=stage_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_key_index_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_index_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_index_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=edge_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_index_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_has_stage_v_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_has_stage_v_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_has_stage_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_has_stage_v_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_result_pipeline_error_type_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/result_pipeline_error_type_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stages\ that\ return\ result<T,\ E>\ must\ use\ a\ compatible\ error\ type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_result_pipeline_error_type_mismatch PROPERTIES LABELS "compile-fail;diagnostics;runtime")
