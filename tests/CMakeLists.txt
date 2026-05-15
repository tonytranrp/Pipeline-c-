if(NOT PB_BUILD_TESTS)
  return()
endif()

add_executable(pb_smoke smoke.cpp)
target_compile_features(pb_smoke PRIVATE cxx_std_20)

add_test(NAME pb_smoke COMMAND pb_smoke)
