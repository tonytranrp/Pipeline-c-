cmake_minimum_required(VERSION 3.23)

project(pipebuilder LANGUAGES CXX)

option(PB_BUILD_TESTS "Build tests" ON)
option(PB_BUILD_EXAMPLES "Build examples" ON)
option(PB_BUILD_BENCH "Build benchmarks" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

if(PB_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if(PB_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

if(PB_BUILD_BENCH)
  add_subdirectory(bench)
endif()
