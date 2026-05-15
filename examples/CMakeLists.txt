add_executable(pb_basic_order_pipeline basic_order_pipeline.cpp)
target_link_libraries(pb_basic_order_pipeline PRIVATE pb::pipeline)
pb_apply_project_options(pb_basic_order_pipeline PRIVATE)

add_executable(pb_error_diagnostic_example error_diagnostic.cpp)
target_link_libraries(pb_error_diagnostic_example PRIVATE pb::pipeline)
pb_apply_project_options(pb_error_diagnostic_example PRIVATE)

add_test(NAME pb_example_basic_order_pipeline COMMAND pb_basic_order_pipeline)
set_tests_properties(pb_example_basic_order_pipeline PROPERTIES LABELS "examples;smoke")
add_test(NAME pb_example_error_diagnostic_compile_fail
  COMMAND ${CMAKE_COMMAND}
    -DTEST_SOURCE=${CMAKE_CURRENT_SOURCE_DIR}/error_diagnostic.cpp
    -DTEST_COMPILER=${CMAKE_CXX_COMPILER}
    -DTEST_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
    -DTEST_DEFINITIONS=PB_EXAMPLE_ENABLE_COMPILE_FAILURE
    -DEXPECT_DIAGNOSTIC=Pipeline\ edge\ mismatch
    -P ${PROJECT_SOURCE_DIR}/tests/run_compile_fail.cmake
)
set_tests_properties(pb_example_error_diagnostic_compile_fail PROPERTIES LABELS "examples;compile-fail")
