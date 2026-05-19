add_executable(pb_policy_dsl_compile_pass compile_pass/policy_dsl.cpp)
target_link_libraries(pb_policy_dsl_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_policy_dsl_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_policy_dsl_compile_pass)
add_test(NAME pb_policy_dsl_compile_pass COMMAND pb_policy_dsl_compile_pass)
set_tests_properties(pb_policy_dsl_compile_pass PROPERTIES LABELS "compile-pass;policy;dsl")

add_executable(pb_smoke_compile_pass compile_pass/smoke_pipeline.cpp)
target_link_libraries(pb_smoke_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_smoke_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_smoke_compile_pass)
add_test(NAME pb_smoke_compile_pass COMMAND pb_smoke_compile_pass)
set_tests_properties(pb_smoke_compile_pass PROPERTIES LABELS "compile-pass;smoke")

add_executable(pb_compile_time_bench_compile_pass compile_pass/compile_time_bench.cpp)
target_link_libraries(pb_compile_time_bench_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_compile_time_bench_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_compile_time_bench_compile_pass)
add_test(NAME pb_compile_time_bench_compile_pass COMMAND pb_compile_time_bench_compile_pass)
set_tests_properties(pb_compile_time_bench_compile_pass PROPERTIES LABELS "compile-pass;bench;baseline")


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

add_executable(pb_core_concepts_false_paths_compile_pass compile_pass/core_concepts_false_paths.cpp)
target_link_libraries(pb_core_concepts_false_paths_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_core_concepts_false_paths_compile_pass PRIVATE)
add_test(NAME pb_core_concepts_false_paths_compile_pass COMMAND pb_core_concepts_false_paths_compile_pass)
set_tests_properties(pb_core_concepts_false_paths_compile_pass PROPERTIES LABELS "compile-pass;concepts")

add_executable(pb_stage_info_compile_pass compile_pass/stage_info.cpp)
target_link_libraries(pb_stage_info_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_stage_info_compile_pass PRIVATE)
add_test(NAME pb_stage_info_compile_pass COMMAND pb_stage_info_compile_pass)
set_tests_properties(pb_stage_info_compile_pass PROPERTIES LABELS "compile-pass;metadata")

add_executable(pb_diagnostics_contract_compile_pass compile_pass/diagnostics_contract.cpp)
target_link_libraries(pb_diagnostics_contract_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_diagnostics_contract_compile_pass PRIVATE)
add_test(NAME pb_diagnostics_contract_compile_pass COMMAND pb_diagnostics_contract_compile_pass)
set_tests_properties(pb_diagnostics_contract_compile_pass PROPERTIES LABELS "compile-pass;diagnostics")

add_executable(pb_stage_naming_descriptor_compile_pass compile_pass/stage_naming_descriptor.cpp)
target_link_libraries(pb_stage_naming_descriptor_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_stage_naming_descriptor_compile_pass PRIVATE)
add_test(NAME pb_stage_naming_descriptor_compile_pass COMMAND pb_stage_naming_descriptor_compile_pass)
set_tests_properties(pb_stage_naming_descriptor_compile_pass PROPERTIES LABELS "compile-pass;metadata")

add_executable(pb_adapt_functor_compile_pass compile_pass/adapt_functor.cpp)
target_link_libraries(pb_adapt_functor_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_adapt_functor_compile_pass PRIVATE)
add_test(NAME pb_adapt_functor_compile_pass COMMAND pb_adapt_functor_compile_pass)
set_tests_properties(pb_adapt_functor_compile_pass PROPERTIES LABELS "compile-pass;adapter")

add_executable(pb_adapt_void_compile_pass compile_pass/adapt_void.cpp)
target_link_libraries(pb_adapt_void_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_adapt_void_compile_pass PRIVATE)
add_test(NAME pb_adapt_void_compile_pass COMMAND pb_adapt_void_compile_pass)
set_tests_properties(pb_adapt_void_compile_pass PROPERTIES LABELS "compile-pass;adapter;void")

add_executable(pb_result_error_type_compatible_compile_pass compile_pass/result_error_type_compatible.cpp)
target_link_libraries(pb_result_error_type_compatible_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_result_error_type_compatible_compile_pass PRIVATE)
add_test(NAME pb_result_error_type_compatible_compile_pass COMMAND pb_result_error_type_compatible_compile_pass)
set_tests_properties(pb_result_error_type_compatible_compile_pass PROPERTIES LABELS "compile-pass;runtime;result")

add_executable(pb_public_headers_compile_pass
  compile_pass/public_headers.cpp
  compile_pass/public_headers/adapt_fn.cpp
  compile_pass/public_headers/adapt_member.cpp
  compile_pass/public_headers/adapt_void.cpp
  compile_pass/public_headers/adaptors.cpp
  compile_pass/public_headers/core_concepts.cpp
  compile_pass/public_headers/core_describe.cpp
  compile_pass/public_headers/core_fixed_string.cpp
  compile_pass/public_headers/core_meta.cpp
  compile_pass/public_headers/core_pipeline_state.cpp
  compile_pass/public_headers/core_stage_traits.cpp
  compile_pass/public_headers/core_validate.cpp
  compile_pass/public_headers/core_diagnostics.cpp
  compile_pass/public_headers/pipeline.cpp
  compile_pass/public_headers/runtime_descriptor.cpp
  compile_pass/public_headers/runtime_backend.cpp
  compile_pass/public_headers/runtime_error.cpp
  compile_pass/public_headers/runtime_result.cpp
  compile_pass/public_headers/runtime_routing.cpp
  compile_pass/public_headers/runtime_sequential.cpp
  compile_pass/public_headers/runtime_observer.cpp
  compile_pass/public_headers/runtime_trace.cpp
  compile_pass/public_headers/export_dot.cpp
  compile_pass/public_headers/export_json.cpp
  compile_pass/branch_selection_compile_pass.cpp
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

add_executable(pb_runtime_descriptor_smoke runtime/descriptor_smoke.cpp)
target_link_libraries(pb_runtime_descriptor_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_descriptor_smoke PRIVATE)
add_test(NAME pb_runtime_descriptor_smoke COMMAND pb_runtime_descriptor_smoke)
set_tests_properties(pb_runtime_descriptor_smoke PROPERTIES LABELS "runtime;metadata")

add_executable(pb_runtime_backend_feature_matrix_smoke runtime/backend_feature_matrix_smoke.cpp)
target_link_libraries(pb_runtime_backend_feature_matrix_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_backend_feature_matrix_smoke PRIVATE)
add_test(NAME pb_runtime_backend_feature_matrix_smoke COMMAND pb_runtime_backend_feature_matrix_smoke)
set_tests_properties(pb_runtime_backend_feature_matrix_smoke PROPERTIES LABELS "runtime;metadata;backend")

add_executable(pb_runtime_thread_pool_smoke runtime/thread_pool_smoke.cpp)
target_link_libraries(pb_runtime_thread_pool_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_thread_pool_smoke PRIVATE)
add_test(NAME pb_runtime_thread_pool_smoke COMMAND pb_runtime_thread_pool_smoke)
set_tests_properties(pb_runtime_thread_pool_smoke PROPERTIES LABELS "runtime;thread-pool;smoke")

add_executable(pb_runtime_sequential_policy_dsl_smoke runtime/sequential_policy_dsl_smoke.cpp)
target_link_libraries(pb_runtime_sequential_policy_dsl_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_policy_dsl_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_policy_dsl_smoke COMMAND pb_runtime_sequential_policy_dsl_smoke)
set_tests_properties(pb_runtime_sequential_policy_dsl_smoke PROPERTIES LABELS "runtime;policy;smoke")

add_executable(pb_runtime_stateful_storage_policy_smoke runtime/stateful_storage_policy_smoke.cpp)
target_link_libraries(pb_runtime_stateful_storage_policy_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_stateful_storage_policy_smoke PRIVATE)
add_test(NAME pb_runtime_stateful_storage_policy_smoke COMMAND pb_runtime_stateful_storage_policy_smoke)
set_tests_properties(pb_runtime_stateful_storage_policy_smoke PROPERTIES LABELS "runtime;policy;stateful")

add_executable(pb_runtime_routing_descriptor_smoke runtime/routing_descriptor_smoke.cpp)
target_link_libraries(pb_runtime_routing_descriptor_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_routing_descriptor_smoke PRIVATE)
add_test(NAME pb_runtime_routing_descriptor_smoke COMMAND pb_runtime_routing_descriptor_smoke)
set_tests_properties(pb_runtime_routing_descriptor_smoke PROPERTIES LABELS "runtime;routing;metadata")

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

add_executable(pb_runtime_sequential_std_expected_policy_smoke runtime/sequential_std_expected_policy_smoke.cpp)
target_link_libraries(pb_runtime_sequential_std_expected_policy_smoke PRIVATE pb::pipeline)
target_compile_features(pb_runtime_sequential_std_expected_policy_smoke PRIVATE cxx_std_23)
pb_apply_project_options(pb_runtime_sequential_std_expected_policy_smoke PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_std_expected_policy_smoke)
add_test(NAME pb_runtime_sequential_std_expected_policy_smoke COMMAND pb_runtime_sequential_std_expected_policy_smoke)
set_tests_properties(pb_runtime_sequential_std_expected_policy_smoke PROPERTIES LABELS "runtime;result;expected")

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

add_executable(pb_runtime_sequential_exception_boundary_smoke runtime/sequential_exception_boundary_smoke.cpp)
target_link_libraries(pb_runtime_sequential_exception_boundary_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_exception_boundary_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_exception_boundary_smoke COMMAND pb_runtime_sequential_exception_boundary_smoke)
set_tests_properties(pb_runtime_sequential_exception_boundary_smoke PROPERTIES LABELS "runtime;diagnostics;observer")

add_executable(pb_runtime_sequential_observer_smoke runtime/sequential_observer_smoke.cpp)
target_link_libraries(pb_runtime_sequential_observer_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_observer_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_observer_smoke COMMAND pb_runtime_sequential_observer_smoke)
set_tests_properties(pb_runtime_sequential_observer_smoke PROPERTIES LABELS "runtime;observer")

add_executable(pb_runtime_trace_export_smoke runtime/trace_export_smoke.cpp)
target_link_libraries(pb_runtime_trace_export_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_trace_export_smoke PRIVATE)
add_test(NAME pb_runtime_trace_export_smoke COMMAND pb_runtime_trace_export_smoke)
set_tests_properties(pb_runtime_trace_export_smoke PROPERTIES LABELS "runtime;trace;observer;export")

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

add_executable(pb_runtime_sequential_branch_smoke runtime/sequential_branch_smoke.cpp)
target_link_libraries(pb_runtime_sequential_branch_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_branch_smoke COMMAND pb_runtime_sequential_branch_smoke)
set_tests_properties(pb_runtime_sequential_branch_smoke PROPERTIES LABELS "runtime;branch;observer;result")

add_executable(pb_runtime_sequential_adapters_smoke runtime/sequential_adapters_smoke.cpp)
target_link_libraries(pb_runtime_sequential_adapters_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_adapters_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_adapters_smoke COMMAND pb_runtime_sequential_adapters_smoke)
set_tests_properties(pb_runtime_sequential_adapters_smoke PROPERTIES LABELS "runtime;adapter")

add_executable(pb_runtime_sequential_adapters_value_category_smoke runtime/sequential_adapters_value_category_smoke.cpp)
target_link_libraries(pb_runtime_sequential_adapters_value_category_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_adapters_value_category_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_adapters_value_category_smoke
  COMMAND pb_runtime_sequential_adapters_value_category_smoke)
set_tests_properties(pb_runtime_sequential_adapters_value_category_smoke PROPERTIES LABELS "runtime;adapter")

add_executable(pb_runtime_adapt_void_smoke runtime/adapt_void_smoke.cpp)
target_link_libraries(pb_runtime_adapt_void_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_adapt_void_smoke PRIVATE)
add_test(NAME pb_runtime_adapt_void_smoke COMMAND pb_runtime_adapt_void_smoke)
set_tests_properties(pb_runtime_adapt_void_smoke PROPERTIES LABELS "runtime;adapter;void")

add_executable(pb_export_dot_compile_pass compile_pass/export_dot.cpp)
target_link_libraries(pb_export_dot_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_export_dot_compile_pass PRIVATE)
add_test(NAME pb_export_dot_compile_pass COMMAND pb_export_dot_compile_pass)
set_tests_properties(pb_export_dot_compile_pass PROPERTIES LABELS "compile-pass;export;dot")

add_executable(pb_export_dot_branch_compile_pass compile_pass/export_dot_branch.cpp)
target_link_libraries(pb_export_dot_branch_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_export_dot_branch_compile_pass PRIVATE)
add_test(NAME pb_export_dot_branch_compile_pass COMMAND pb_export_dot_branch_compile_pass)
set_tests_properties(pb_export_dot_branch_compile_pass PROPERTIES LABELS "compile-pass;export;dot;branch")

add_executable(pb_export_json_compile_pass compile_pass/export_json.cpp)
target_link_libraries(pb_export_json_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_export_json_compile_pass PRIVATE)
add_test(NAME pb_export_json_compile_pass COMMAND pb_export_json_compile_pass)
set_tests_properties(pb_export_json_compile_pass PROPERTIES LABELS "compile-pass;export;json;branch")

add_executable(pb_export_json_branch_compile_pass compile_pass/export_json_branch.cpp)
target_link_libraries(pb_export_json_branch_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_export_json_branch_compile_pass PRIVATE)
add_test(NAME pb_export_json_branch_compile_pass COMMAND pb_export_json_branch_compile_pass)
set_tests_properties(pb_export_json_branch_compile_pass PROPERTIES LABELS "compile-pass;export;json;branch")

add_executable(pb_export_golden_compile_pass compile_pass/export_golden.cpp)
target_link_libraries(pb_export_golden_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_export_golden_compile_pass PRIVATE)
add_test(NAME pb_export_golden_compile_pass COMMAND pb_export_golden_compile_pass)
set_tests_properties(pb_export_golden_compile_pass PROPERTIES LABELS "compile-pass;export;golden;branch")

add_executable(pb_branch_join_validation_compile_pass compile_pass/branch_join_validation_compile_pass.cpp)
target_link_libraries(pb_branch_join_validation_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_branch_join_validation_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_branch_join_validation_compile_pass)
add_test(NAME pb_branch_join_validation_compile_pass COMMAND pb_branch_join_validation_compile_pass)
set_tests_properties(pb_branch_join_validation_compile_pass PROPERTIES LABELS "compile-pass;metadata;topology")

add_executable(pb_branch_heterogeneous_compile_pass compile_pass/branch_heterogeneous_compile_pass.cpp)
target_link_libraries(pb_branch_heterogeneous_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_branch_heterogeneous_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_branch_heterogeneous_compile_pass)
add_test(NAME pb_branch_heterogeneous_compile_pass COMMAND pb_branch_heterogeneous_compile_pass)
set_tests_properties(pb_branch_heterogeneous_compile_pass PROPERTIES LABELS "compile-pass;branch;heterogeneous")

add_executable(pb_branch_topology_heterogeneous_compile_pass compile_pass/branch_topology_heterogeneous_compile_pass.cpp)
target_link_libraries(pb_branch_topology_heterogeneous_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_branch_topology_heterogeneous_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_branch_topology_heterogeneous_compile_pass)
add_test(NAME pb_branch_topology_heterogeneous_compile_pass COMMAND pb_branch_topology_heterogeneous_compile_pass)
set_tests_properties(pb_branch_topology_heterogeneous_compile_pass PROPERTIES LABELS "compile-pass;branch;heterogeneous;topology")

add_executable(pb_branch_type_list_join_compile_pass compile_pass/branch_type_list_join_compile_pass.cpp)
target_link_libraries(pb_branch_type_list_join_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_branch_type_list_join_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_branch_type_list_join_compile_pass)
add_test(NAME pb_branch_type_list_join_compile_pass COMMAND pb_branch_type_list_join_compile_pass)
set_tests_properties(pb_branch_type_list_join_compile_pass PROPERTIES LABELS "compile-pass;branch;join;type-list")

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

add_test(NAME pb_compile_fail_duplicate_stage_edge_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/duplicate_stage_edge_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -DEXPECTED_DIAGNOSTICS=pb::AdjacentStages
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_duplicate_stage_edge_mismatch PROPERTIES LABELS "compile-fail;diagnostics")

# (removed: branch_topology_unsupported_boundary — heterogeneous branch now compiles via std::variant)
# (removed: branch_outputs_heterogeneous_join_unsupported — heterogeneous join now supported via std::variant)

add_executable(pb_runtime_sequential_branch_execution_smoke runtime/sequential_branch_execution_smoke.cpp)
target_link_libraries(pb_runtime_sequential_branch_execution_smoke PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_execution_smoke PRIVATE)
add_test(NAME pb_runtime_sequential_branch_execution_smoke COMMAND pb_runtime_sequential_branch_execution_smoke)
set_tests_properties(pb_runtime_sequential_branch_execution_smoke PROPERTIES LABELS "runtime;observer;branch")

add_executable(pb_runtime_sequential_branch_move_only runtime/sequential_branch_move_only.cpp)
target_link_libraries(pb_runtime_sequential_branch_move_only PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_move_only PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_branch_move_only)
add_test(NAME pb_runtime_sequential_branch_move_only COMMAND pb_runtime_sequential_branch_move_only)
set_tests_properties(pb_runtime_sequential_branch_move_only PROPERTIES LABELS "runtime;branch;move-only")

add_executable(pb_runtime_sequential_branch_comprehensive runtime/sequential_branch_comprehensive.cpp)
target_link_libraries(pb_runtime_sequential_branch_comprehensive PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_comprehensive PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_branch_comprehensive)
add_test(NAME pb_runtime_sequential_branch_comprehensive COMMAND pb_runtime_sequential_branch_comprehensive)
set_tests_properties(pb_runtime_sequential_branch_comprehensive PROPERTIES LABELS "runtime;branch;observer;result;comprehensive")

add_executable(pb_runtime_sequential_branch_unnamed_stages runtime/sequential_branch_unnamed_stages.cpp)
target_link_libraries(pb_runtime_sequential_branch_unnamed_stages PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_unnamed_stages PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_branch_unnamed_stages)
add_test(NAME pb_runtime_sequential_branch_unnamed_stages COMMAND pb_runtime_sequential_branch_unnamed_stages)
set_tests_properties(pb_runtime_sequential_branch_unnamed_stages PROPERTIES LABELS "runtime;branch;observer")

add_executable(pb_runtime_sequential_branch_heterogeneous runtime/sequential_branch_heterogeneous.cpp)
target_link_libraries(pb_runtime_sequential_branch_heterogeneous PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_heterogeneous PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_branch_heterogeneous)
add_test(NAME pb_runtime_sequential_branch_heterogeneous COMMAND pb_runtime_sequential_branch_heterogeneous)
set_tests_properties(pb_runtime_sequential_branch_heterogeneous PROPERTIES LABELS "runtime;branch;heterogeneous")

add_executable(pb_runtime_sequential_branch_type_list_join runtime/sequential_branch_type_list_join.cpp)
target_link_libraries(pb_runtime_sequential_branch_type_list_join PRIVATE pb::pipeline)
pb_apply_project_options(pb_runtime_sequential_branch_type_list_join PRIVATE)
pb_enable_pch_if_requested(pb_runtime_sequential_branch_type_list_join)
add_test(NAME pb_runtime_sequential_branch_type_list_join COMMAND pb_runtime_sequential_branch_type_list_join)
set_tests_properties(pb_runtime_sequential_branch_type_list_join PROPERTIES LABELS "runtime;branch;join;type-list")

add_test(NAME pb_compile_fail_branch_builder_source_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_builder_source_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ builder\ source\ mismatch
    -DEXPECTED_DIAGNOSTICS=current\ pipeline\ output_type;branch\ case\ input_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_builder_source_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")


add_test(NAME pb_compile_fail_branch_builder_empty_cases_unsupported
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_builder_empty_cases_unsupported.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ node\ requires\ at\ least\ one
    -DEXPECTED_DIAGNOSTICS=at\ least\ one;case_
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_builder_empty_cases_unsupported PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")
add_test(NAME pb_compile_fail_branch_case_invalid_predicate_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_case_invalid_predicate_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ predicate\ is\ invalid
    -DEXPECTED_DIAGNOSTICS=bool-like\ output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_case_invalid_predicate_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_case_predicate_output_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_case_predicate_output_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ predicate\ stage\ must\ produce\ a\ bool-like\ output_type
    -DEXPECTED_DIAGNOSTICS=branch_case
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_case_predicate_output_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_case_input_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_case_input_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ case\ source\ mismatch
    -DEXPECTED_DIAGNOSTICS=predicate\ input_type;branch\ target\ stage\ input_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_case_input_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_outputs_invalid_case_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_outputs_invalid_case_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ outputs\ require\ pb::case_
    -DEXPECTED_DIAGNOSTICS=branch_outputs
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_outputs_invalid_case_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_outputs_case_input_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_outputs_case_input_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ outputs\ source\ mismatch
    -DEXPECTED_DIAGNOSTICS=branch_outputs;input_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_outputs_case_input_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_outputs_empty_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_outputs_empty_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ outputs\ require\ at\ least\ one\ pb::case_
    -DEXPECTED_DIAGNOSTICS=branch_outputs
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_outputs_empty_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_validation_input_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_validation_input_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ validation\ mismatch
    -DEXPECTED_DIAGNOSTICS=Join\ validation;join\ stage\ input_type;branch\ output_types
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_validation_input_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_output_validation_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_output_validation_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ output\ validation\ mismatch
    -DEXPECTED_DIAGNOSTICS=branch_output_validation;output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_output_validation_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_unified_output_validation_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_unified_output_validation_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ unified\ output\ validation\ mismatch
    -DEXPECTED_DIAGNOSTICS=branch_unified_output_validation;branch_outputs::output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_unified_output_validation_mismatch
  PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology;branch;heterogeneous")

add_test(NAME pb_compile_fail_branch_case_output_non_case_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_case_output_non_case_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ output\ marker\ requires\ pb::case_
    -DEXPECTED_DIAGNOSTICS=branch_case_output;branch_node
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_case_output_non_case_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_node_case_input_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_node_case_input_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ node\ source\ mismatch
    -DEXPECTED_DIAGNOSTICS=branch_node;input_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_node_case_input_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_outputs_join_node_boundary_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_outputs_join_node_boundary_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Branch\ outputs\ require\ pb::case_
    -DEXPECTED_DIAGNOSTICS=branch_outputs;join_node
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_outputs_join_node_boundary_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_node_invalid_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_node_invalid_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ stage\ is\ invalid
    -DEXPECTED_DIAGNOSTICS=input_type;output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_node_invalid_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_builder_source_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_builder_source_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ builder\ source\ mismatch
    -DEXPECTED_DIAGNOSTICS=join\ stage\ input_type;branch\ output_types
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_builder_source_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_without_branch_diagnostic
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_without_branch_diagnostic.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join requires a preceding branch
    -DEXPECTED_DIAGNOSTICS=pipeline_state;branch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_without_branch_diagnostic PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_join_type_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_join_type_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline edge mismatch
    -DEXPECTED_DIAGNOSTICS=unified branch output_type;type-list joins must be invocable
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_join_type_mismatch PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_branch_heterogeneous_join_type_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_heterogeneous_join_type_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -DEXPECTED_DIAGNOSTICS=unified branch output_type;type-list joins must be invocable
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_heterogeneous_join_type_mismatch PROPERTIES LABELS "compile-fail;diagnostics;branch;heterogeneous")

add_executable(pb_branch_move_only_input_compile_pass compile_pass/branch_move_only_input.cpp)
target_link_libraries(pb_branch_move_only_input_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_branch_move_only_input_compile_pass PRIVATE)
pb_enable_pch_if_requested(pb_branch_move_only_input_compile_pass)
add_test(NAME pb_branch_move_only_input_compile_pass COMMAND pb_branch_move_only_input_compile_pass)
set_tests_properties(pb_branch_move_only_input_compile_pass PROPERTIES LABELS "compile-pass;branch;move-only")

add_test(NAME pb_compile_fail_branch_move_only_predicate_by_value_unsupported
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_move_only_predicate_by_value_unsupported.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Move-only\ branch\ inputs\ require\ predicates\ callable\ with\ const\ input_type&
    -DEXPECTED_DIAGNOSTICS=predicates
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_move_only_predicate_by_value_unsupported
  PROPERTIES LABELS "compile-fail;diagnostics;branch;move-only")

add_test(NAME pb_compile_fail_branch_type_list_join_missing_handler
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/branch_type_list_join_missing_handler.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=type-list\ joins\ must\ be\ invocable\ for\ every\ branch\ output_type
    -DEXPECTED_DIAGNOSTICS=Pipeline\ edge\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_branch_type_list_join_missing_handler
  PROPERTIES LABELS "compile-fail;diagnostics;branch;join;type-list")

add_test(NAME pb_compile_fail_join_output_invalid_node_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_output_invalid_node_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ output\ marker\ requires\ pb::join_node
    -DEXPECTED_DIAGNOSTICS=join_output;JoinStage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_output_invalid_node_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_output_branch_outputs_boundary_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_output_branch_outputs_boundary_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ output\ marker\ requires\ pb::join_node
    -DEXPECTED_DIAGNOSTICS=join_output;branch_outputs
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_output_branch_outputs_boundary_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_validation_invalid_outputs_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_validation_invalid_outputs_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ validation\ requires\ pb::branch_outputs
    -DEXPECTED_DIAGNOSTICS=pb::branch_outputs;pb::join_node
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_validation_invalid_outputs_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

add_test(NAME pb_compile_fail_join_validation_invalid_join_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/join_validation_invalid_join_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Join\ validation\ requires\ pb::branch_outputs
    -DEXPECTED_DIAGNOSTICS=pb::branch_outputs;pb::join_node
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_join_validation_invalid_join_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata;topology")

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

add_test(NAME pb_compile_fail_adapt_functor_non_default_initializable
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_functor_non_default_initializable.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ callable\ object\ must\ be\ default-initializable
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_functor_non_default_initializable PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_direct_member_non_default_initializable
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_direct_member_non_default_initializable.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Adapted\ stage\ member\ object\ must\ be\ default-initializable
    -DEXPECTED_DIAGNOSTICS=adapted_stage
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_direct_member_non_default_initializable PROPERTIES LABELS "compile-fail;diagnostics;adapter")

add_test(NAME pb_compile_fail_adapt_non_final_void_stage_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/adapt_non_final_void_stage_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -DEXPECTED_DIAGNOSTICS=pb::connectable_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_adapt_non_final_void_stage_mismatch PROPERTIES LABELS "compile-fail;diagnostics;adapter;runtime")

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

add_test(NAME pb_compile_fail_stage_input_invalid_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/stage_input_invalid_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=stage_input_t
    -DEXPECTED_DIAGNOSTICS=input_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_stage_input_invalid_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_stage_output_invalid_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/stage_output_invalid_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=stage_output_t
    -DEXPECTED_DIAGNOSTICS=output_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_stage_output_invalid_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_stage_error_invalid_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/stage_error_invalid_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=stage_error_t
    -DEXPECTED_DIAGNOSTICS=error_type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_stage_error_invalid_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_stage_descriptor_invalid_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/stage_descriptor_invalid_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=stage_descriptor
    -DEXPECTED_DIAGNOSTICS=stage_input_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_stage_descriptor_invalid_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_edge_descriptor_invalid_from_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_descriptor_invalid_from_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=edge_descriptor
    -DEXPECTED_DIAGNOSTICS=stage_output_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_edge_descriptor_invalid_from_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_edge_descriptor_invalid_to_stage_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/edge_descriptor_invalid_to_stage_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=edge_descriptor
    -DEXPECTED_DIAGNOSTICS=stage_input_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_edge_descriptor_invalid_to_stage_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_sink_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/sink_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ sink\ mismatch
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
  )
set_tests_properties(pb_compile_fail_sink_mismatch PROPERTIES LABELS "compile-fail;diagnostics")

add_test(NAME pb_compile_fail_zero_stage_sink_mismatch
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/zero_stage_sink_mismatch.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ sink\ mismatch
    -DEXPECTED_DIAGNOSTICS=requested\ sink\ type
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
  )
set_tests_properties(pb_compile_fail_zero_stage_sink_mismatch PROPERTIES LABELS "compile-fail;diagnostics")


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

add_test(NAME pb_compile_fail_pipeline_stage_type_zero_stage_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_type_zero_stage_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=stage_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_type_zero_stage_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_descriptor_zero_stage_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_descriptor_zero_stage_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=stage_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_descriptor_zero_stage_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_name_zero_stage_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_name_zero_stage_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=stage_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_name_zero_stage_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_key_zero_stage_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_key_zero_stage_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ stage\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=stage_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_key_zero_stage_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_alias_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_alias_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_stage_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_alias_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_descriptor_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_descriptor_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_stage_descriptor_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_descriptor_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_error_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_error_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_stage_error_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_error_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_size_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_size_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_size_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_size_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_empty_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_empty_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_empty_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_empty_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_input_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_input_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_input_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_input_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_output_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_output_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_output_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_output_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stages_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stages_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_stages_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stages_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_key_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_key_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=stage_key
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_key_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_stage_name_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_stage_name_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=stage_name
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_stage_name_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

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

add_test(NAME pb_compile_fail_pipeline_edge_from_key_zero_edge_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_from_key_zero_edge_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=edge_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_from_key_zero_edge_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_from_stage_index_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_from_stage_index_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=edge_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_from_stage_index_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_to_stage_zero_edge_oob
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_to_stage_zero_edge_oob.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ metadata\ index\ out\ of\ range
    -DEXPECTED_DIAGNOSTICS=edge_count
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_to_stage_zero_edge_oob PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_alias_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_alias_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_edge_descriptor_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_alias_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_from_stage_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_from_stage_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_edge_from_stage_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_from_stage_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_to_stage_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_to_stage_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_edge_to_stage_t
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_to_stage_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_count_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_count_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=pipeline_edge_count_v
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_count_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_from_key_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_from_key_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=edge_from_key
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_from_key_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_from_name_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_from_name_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=edge_from_name
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_from_name_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_to_key_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_to_key_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=edge_to_key
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_to_key_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_edge_to_name_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_edge_to_name_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=edge_to_name
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_edge_to_name_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_descriptor_view_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_descriptor_view_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=descriptor_view
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_descriptor_view_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

add_test(NAME pb_compile_fail_pipeline_describe_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/pipeline_describe_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -DEXPECTED_DIAGNOSTICS=describe
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_pipeline_describe_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;metadata")

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

add_test(NAME pb_compile_fail_runtime_make_descriptor_non_pipeline_misuse
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/runtime_make_descriptor_non_pipeline_misuse.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DEXPECT_DIAGNOSTIC=ValidPipeline
    -P ${CMAKE_CURRENT_SOURCE_DIR}/run_compile_fail.cmake
)
set_tests_properties(pb_compile_fail_runtime_make_descriptor_non_pipeline_misuse PROPERTIES LABELS "compile-fail;diagnostics;runtime")

# ─────────────────────────────────────────────────────────────
# C++26 feature‑gate compile‑pass test
# ─────────────────────────────────────────────────────────────
add_executable(pb_cpp26_features_compile_pass compile_pass/cpp26_features.cpp)
target_link_libraries(pb_cpp26_features_compile_pass PRIVATE pb::pipeline)
pb_apply_project_options(pb_cpp26_features_compile_pass PRIVATE)
add_test(NAME pb_cpp26_features_compile_pass COMMAND pb_cpp26_features_compile_pass)
set_tests_properties(pb_cpp26_features_compile_pass PROPERTIES LABELS "compile-pass;features;cpp26")

# ─────────────────────────────────────────────────────────────
# CLI smoke tests
# ─────────────────────────────────────────────────────────────
if(PB_BUILD_CLI)
  add_test(NAME pb_cli_no_args     COMMAND pb_cli)
  set_tests_properties(pb_cli_no_args PROPERTIES LABELS "cli;smoke")

  add_test(NAME pb_cli_version     COMMAND pb_cli version)
  set_tests_properties(pb_cli_version PROPERTIES LABELS "cli;smoke")

  add_test(NAME pb_cli_features    COMMAND pb_cli features)
  set_tests_properties(pb_cli_features PROPERTIES LABELS "cli;features")

  add_test(NAME pb_cli_export      COMMAND pb_cli export)
  set_tests_properties(pb_cli_export PROPERTIES LABELS "cli;export")

  add_test(NAME pb_cli_export_dot  COMMAND pb_cli export --dot)
  set_tests_properties(pb_cli_export_dot PROPERTIES LABELS "cli;export;dot")

  add_test(NAME pb_cli_export_json COMMAND pb_cli export --json)
  set_tests_properties(pb_cli_export_json PROPERTIES LABELS "cli;export;json")

  add_test(NAME pb_cli_validate    COMMAND pb_cli validate)
  set_tests_properties(pb_cli_validate PROPERTIES LABELS "cli;reserved")

  add_test(NAME pb_cli_unknown     COMMAND pb_cli nonexistent_command)
  set_tests_properties(pb_cli_unknown PROPERTIES LABELS "cli;error")
  set_tests_properties(pb_cli_unknown PROPERTIES WILL_FAIL TRUE)
endif()
