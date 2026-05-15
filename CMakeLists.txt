cmake_minimum_required(VERSION 3.24)

project(PipelineCpp LANGUAGES CXX)

include(CTest)

option(PB_BUILD_TESTS "Build test targets" ON)
option(PB_BUILD_BENCHMARKS "Build benchmark targets" ON)
option(PB_BUILD_DOCS "Build docs targets" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(tests)

if(PB_BUILD_BENCHMARKS)
  add_subdirectory(bench)
endif()

if(PB_BUILD_DOCS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs/CMakeLists.txt")
  add_subdirectory(docs)
endif()
