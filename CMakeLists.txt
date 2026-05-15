cmake_minimum_required(VERSION 3.24)

project(PipelineCpp LANGUAGES CXX)

include(CTest)

option(PB_BUILD_TESTS "Build test targets" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(pb_runtime INTERFACE)
target_include_directories(pb_runtime INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(pb_runtime INTERFACE cxx_std_20)

if(PB_BUILD_TESTS)
  add_subdirectory(tests)
endif()

