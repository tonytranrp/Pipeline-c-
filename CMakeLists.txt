cmake_minimum_required(VERSION 3.23)

project(
  pipebuilder
  VERSION 0.1.0
  DESCRIPTION "Compile-time validated C++ pipeline builder"
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not supported. Use a separate build directory, such as a CMake preset or `cmake -S . -B build/dev`.")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/PBCompilerOptions.cmake)

option(PB_BUILD_TESTS "Build pipebuilder tests" ON)
option(PB_BUILD_EXAMPLES "Build pipebuilder examples" ON)
option(PB_BUILD_BENCHMARKS "Build pipebuilder benchmark smoke targets" OFF)
option(PB_BUILD_CLI "Build pipebuilder CLI tool (pb_cli)" ON)
option(PB_ENABLE_WARNINGS "Enable project warning flags" ON)
option(PB_WARNINGS_AS_ERRORS "Treat project warnings as errors" OFF)
option(PB_ENABLE_UNITY "Enable unity builds for implementation targets" OFF)
option(PB_ENABLE_PCH "Enable precompiled headers for examples/tests that opt in" OFF)
option(PB_ENABLE_IPO "Enable interprocedural optimization when supported" OFF)
option(PB_ENABLE_CLANG_TIME_TRACE "Enable Clang -ftime-trace compile-time profiling" OFF)
option(PB_ENABLE_CLANG_TIDY "Enable clang-tidy for compiled project targets when available" OFF)
option(PB_ENABLE_PACKAGE_SMOKE "Enable CTest smoke coverage for the installed CMake package" ON)
option(PB_BACKEND_TASKFLOW "Enable optional Taskflow backend integration" OFF)
option(PB_BACKEND_TBB "Enable optional oneTBB backend integration" OFF)
option(PB_BACKEND_STDEXEC "Enable experimental stdexec backend integration" OFF)

if(PB_BACKEND_TASKFLOW OR PB_BACKEND_TBB OR PB_BACKEND_STDEXEC)
  message(FATAL_ERROR
    "Optional external backend adapters are reserved but not implemented in this release. "
    "Keep PB_BACKEND_TASKFLOW, PB_BACKEND_TBB, and PB_BACKEND_STDEXEC OFF; "
    "the supported runtime backend is the standard-library sequential runtime."
  )
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compile_commands.json for tooling")

add_library(pb_core INTERFACE)
add_library(pb::core ALIAS pb_core)
set_target_properties(pb_core PROPERTIES EXPORT_NAME core)
target_compile_features(pb_core INTERFACE cxx_std_20)
target_include_directories(
  pb_core
  INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
pb_apply_project_options(pb_core INTERFACE)

add_library(pb_runtime
  src/runtime/error.cpp
  src/runtime/sequential.cpp
)
add_library(pb::runtime ALIAS pb_runtime)
add_library(pb::pipeline ALIAS pb_runtime)
set_target_properties(pb_runtime PROPERTIES EXPORT_NAME runtime)
target_link_libraries(pb_runtime PUBLIC pb_core)
pb_apply_project_options(pb_runtime PRIVATE)
pb_enable_clang_tidy_if_requested(pb_runtime)
if(PB_ENABLE_UNITY)
  set_target_properties(pb_runtime PROPERTIES UNITY_BUILD ON)
endif()
pb_enable_ipo_if_requested(pb_runtime)

if(PB_BUILD_CLI)
  add_executable(pb_cli tools/pb_cli.cpp)
  target_link_libraries(pb_cli PRIVATE pb::pipeline)
  pb_apply_project_options(pb_cli PRIVATE)
  pb_enable_clang_tidy_if_requested(pb_cli)
  if(PB_ENABLE_UNITY)
    set_target_properties(pb_cli PROPERTIES UNITY_BUILD ON)
  endif()
  pb_enable_ipo_if_requested(pb_cli)
endif()

if(PB_BUILD_TESTS OR PB_BUILD_EXAMPLES OR PB_BUILD_BENCHMARKS)
  enable_testing()
endif()

if(PB_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PB_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(PB_BUILD_TESTS AND PB_ENABLE_PACKAGE_SMOKE)
  configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/PBPackageSmoke.cmake.in"
    "${PROJECT_BINARY_DIR}/PBPackageSmoke.cmake"
    @ONLY
  )
  add_test(
    NAME pb_package_config_smoke
    COMMAND ${CMAKE_COMMAND}
      -DPB_PACKAGE_SMOKE_CONFIG=$<CONFIG>
      -P "${PROJECT_BINARY_DIR}/PBPackageSmoke.cmake"
  )
  set_tests_properties(pb_package_config_smoke PROPERTIES LABELS "package;smoke")
endif()

if(PB_BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()

install(
  TARGETS pb_core pb_runtime
  EXPORT pipebuilderTargets
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")

write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/pipebuilderConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/pipebuilderConfig.cmake.in"
  "${PROJECT_BINARY_DIR}/pipebuilderConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pipebuilder
)
install(
  EXPORT pipebuilderTargets
  FILE pipebuilderTargets.cmake
  NAMESPACE pb::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pipebuilder
)
install(
  FILES
    "${PROJECT_BINARY_DIR}/pipebuilderConfig.cmake"
    "${PROJECT_BINARY_DIR}/pipebuilderConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/pipebuilder
)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "pipebuilder")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
include(CPack)
