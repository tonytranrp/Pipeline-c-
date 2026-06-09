# Runs `pb_cli <COMMAND>` (or caller-supplied COMMAND_ARGS) and checks stdout contains EXPECTED_TOKENS.
# Inputs via -D:
#   CLI_PATH         absolute path to the pb_cli executable
#   COMMAND          command argument passed to pb_cli (legacy single-arg form)
#   COMMAND_ARGS     optional semicolon-separated argument list passed to pb_cli
#   EXPECTED_TOKENS  semicolon-separated substrings expected in stdout

if(NOT DEFINED CLI_PATH)
  message(FATAL_ERROR "CLI_PATH is required")
endif()
if(NOT DEFINED COMMAND_ARGS)
  if(NOT DEFINED COMMAND)
    message(FATAL_ERROR "COMMAND or COMMAND_ARGS is required")
  endif()
  set(COMMAND_ARGS "${COMMAND}")
endif()
if(NOT DEFINED EXPECTED_TOKENS)
  message(FATAL_ERROR "EXPECTED_TOKENS is required")
endif()

execute_process(
  COMMAND "${CLI_PATH}" ${COMMAND_ARGS}
  RESULT_VARIABLE cli_result
  OUTPUT_VARIABLE cli_stdout
  ERROR_VARIABLE cli_stderr
)

if(NOT cli_result EQUAL 0)
  message(FATAL_ERROR
    "pb_cli ${COMMAND_ARGS} failed with status ${cli_result}.
"
    "stdout: ${cli_stdout}
stderr: ${cli_stderr}")
endif()

foreach(token IN LISTS EXPECTED_TOKENS)
  string(FIND "${cli_stdout}" "${token}" found_at)
  if(found_at EQUAL -1)
    message(FATAL_ERROR
      "pb_cli ${COMMAND_ARGS} is missing expected token '${token}'.
"
      "Output:
${cli_stdout}")
  endif()
endforeach()
