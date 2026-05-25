# Runs `pb_cli describe <PIPELINE_NAME> --format=<FORMAT> --out=<OUT_FILE>`,
# verifies the executable exits with the expected status, and confirms the
# output file contains every substring listed in EXPECTED_TOKENS.
#
# Inputs:
#   CLI_PATH         absolute path to the pb_cli executable
#   PIPELINE_NAME    pipeline name argument passed to `describe`
#   FORMAT           "dot" | "json"
#   OUT_FILE         absolute path where the CLI writes its output
#   EXPECTED_TOKENS  semicolon-separated list of substrings that must appear
#                    in the generated output file
#   EXPECT_FAIL      (optional) if "ON", expect a non-zero exit code
#                    and skip content checks
#
# All inputs must be passed via -D<NAME>=<VALUE>.

if(NOT DEFINED CLI_PATH)
  message(FATAL_ERROR "CLI_PATH is required")
endif()
if(NOT DEFINED PIPELINE_NAME)
  message(FATAL_ERROR "PIPELINE_NAME is required")
endif()
if(NOT DEFINED FORMAT)
  set(FORMAT "dot")
endif()
if(NOT DEFINED OUT_FILE)
  message(FATAL_ERROR "OUT_FILE is required")
endif()
if(NOT DEFINED EXPECT_FAIL)
  set(EXPECT_FAIL "OFF")
endif()

if(EXISTS "${OUT_FILE}")
  file(REMOVE "${OUT_FILE}")
endif()

execute_process(
  COMMAND "${CLI_PATH}" describe "${PIPELINE_NAME}" "--format=${FORMAT}" "--out=${OUT_FILE}"
  RESULT_VARIABLE cli_result
  OUTPUT_VARIABLE cli_stdout
  ERROR_VARIABLE cli_stderr
)

if(EXPECT_FAIL)
  if(cli_result EQUAL 0)
    message(FATAL_ERROR
      "Expected pb_cli describe '${PIPELINE_NAME}' to fail, but it succeeded.\n"
      "stdout: ${cli_stdout}\nstderr: ${cli_stderr}")
  endif()
  return()
endif()

if(NOT cli_result EQUAL 0)
  message(FATAL_ERROR
    "pb_cli describe '${PIPELINE_NAME}' failed with status ${cli_result}.\n"
    "stdout: ${cli_stdout}\nstderr: ${cli_stderr}")
endif()

if(NOT EXISTS "${OUT_FILE}")
  message(FATAL_ERROR "pb_cli describe did not produce '${OUT_FILE}'.")
endif()

file(READ "${OUT_FILE}" generated_output)

foreach(token IN LISTS EXPECTED_TOKENS)
  string(FIND "${generated_output}" "${token}" found_at)
  if(found_at EQUAL -1)
    message(FATAL_ERROR
      "pb_cli describe '${PIPELINE_NAME}' (--format=${FORMAT}) is missing expected token '${token}'.\n"
      "Output file: ${OUT_FILE}\n"
      "Output:\n${generated_output}")
  endif()
endforeach()
