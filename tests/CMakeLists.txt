if(NOT PB_BUILD_TESTS)
  return()
endif()

add_executable(pb_smoke smoke.cpp)
target_compile_features(pb_smoke PRIVATE cxx_std_20)
target_link_libraries(pb_smoke PRIVATE pb_runtime)

add_test(NAME pb_smoke COMMAND pb_smoke)

