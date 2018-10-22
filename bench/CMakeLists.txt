#set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
set(BENCHMARK_ENABLE_TESTING OFF)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in benchmark-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bench/benchmark-download )
if(result)
  message(FATAL_ERROR "CMake step for benchmark failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bench/benchmark-download )
if(result)
  message(FATAL_ERROR "Build step for benchmark failed: ${result}")
endif()

add_subdirectory(${CMAKE_BINARY_DIR}/bench/benchmark-src
    ${CMAKE_BINARY_DIR}/bench/benchmark-build)

set(BENCH_FILES matrix_bench.cpp)
add_executable(correrBenchmarks ${BENCH_FILES})
target_include_directories(correrBenchmarks PUBLIC ${HEADERS_DIR})
target_link_libraries(correrBenchmarks benchmark)