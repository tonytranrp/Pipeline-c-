# Runs `pb_cli list`, verifies it exits 0, and confirms its stdout contains
# every substring listed in EXPECTED_TOKENS.  Used to assert that pipelines
# registered through pb::tooling::pipeline_registry surface in the CLI's
# `list` output (modeled on run_pb_cli_describe.cmake, but checks stdout
# instead of an --out file since `list` is a stdout-only command).
#
# Inputs (all via -D<NAME>=<VALUE>):
#   CLI_PATH         absolute path to the pb_cli executable
#   EXPECTED_TOKENS  semicolon-separated list of substrings that must appear
#                    in `pb_cli list` stdout

if(NOT DEFINED CLI_PATH)
  message(FATAL_ERROR "CLI_PATH is required")
endif()
if(NOT DEFINED EXPECTED_TOKENS)
  message(FATAL_ERROR "EXPECTED_TOKENS is required")
endif()

execute_process(
  COMMAND "${CLI_PATH}" list
  RESULT_VARIABLE cli_result
  OUTPUT_VARIABLE cli_stdout
  ERROR_VARIABLE cli_stderr
)

if(NOT cli_result EQUAL 0)
  message(FATAL_ERROR
    "pb_cli list failed with status ${cli_result}.\n"
    "stdout: ${cli_stdout}\nstderr: ${cli_stderr}")
endif()

foreach(token IN LISTS EXPECTED_TOKENS)
  string(FIND "${cli_stdout}" "${token}" found_at)
  if(found_at EQUAL -1)
    message(FATAL_ERROR
      "pb_cli list is missing expected token '${token}'.\n"
      "Output:\n${cli_stdout}")
  endif()
endforeach()
