if(NOT DEFINED TEST_SOURCE)
  message(FATAL_ERROR "TEST_SOURCE is required")
endif()

get_filename_component(test_dir "${TEST_SOURCE}" DIRECTORY)
get_filename_component(test_name "${TEST_SOURCE}" NAME_WE)
set(output_file "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.log")

if(NOT DEFINED TEST_COMPILER)
  message(FATAL_ERROR "TEST_COMPILER is required")
endif()

if(NOT DEFINED TEST_INCLUDE_DIR)
  message(FATAL_ERROR "TEST_INCLUDE_DIR is required")
endif()

if(NOT DEFINED TEST_EXPECTED_DIAGNOSTIC)
  set(TEST_EXPECTED_DIAGNOSTIC "Pipeline edge mismatch")
endif()

if(WIN32 AND TEST_COMPILER MATCHES "cl(\\.exe)?$")
  set(compile_cmd "${TEST_COMPILER}" /std:c++20 /nologo /I "${TEST_INCLUDE_DIR}" /c "${TEST_SOURCE}")
else()
  set(compile_cmd "${TEST_COMPILER}" -std=c++20 -I "${TEST_INCLUDE_DIR}" -fsyntax-only "${TEST_SOURCE}")
endif()

execute_process(
  COMMAND ${compile_cmd}
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
  ERROR_VARIABLE stderr
)

file(WRITE "${output_file}" "${stdout}\n${stderr}\n")

if(result EQUAL 0)
  message(FATAL_ERROR "Expected compile failure for ${test_name}")
endif()

string(FIND "${stdout}\n${stderr}" "${TEST_EXPECTED_DIAGNOSTIC}" diagnostic_found)
if(diagnostic_found EQUAL -1)
  message(FATAL_ERROR "Missing expected diagnostic text for ${test_name}: ${TEST_EXPECTED_DIAGNOSTIC}")
endif()
