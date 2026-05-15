add_executable(pb_bench_compile_time compile_time/include_pipeline.cpp)
target_include_directories(pb_bench_compile_time PRIVATE "${PROJECT_SOURCE_DIR}/include")

add_executable(pb_bench_compile_time_50 compile_time/pipeline_50_stage.cpp)
target_include_directories(pb_bench_compile_time_50 PRIVATE "${PROJECT_SOURCE_DIR}/include")

add_executable(pb_bench_runtime runtime/sequential_5_stage.cpp)
target_include_directories(pb_bench_runtime PRIVATE "${PROJECT_SOURCE_DIR}/include")
