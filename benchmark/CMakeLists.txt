cmake_minimum_required(VERSION 3.27.1)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    messgae(FATAL_ERROR "benchmarks depend on linux to count cpu cycles")
endif()

set(BENCH_BYTESTREAM "${CMAKE_CURRENT_SOURCE_DIR}/byteStreamBench.c")
set(BENCH_BYTEUNICODE "${CMAKE_CURRENT_SOURCE_DIR}/byteUnicodeBench.c")
set(BENCH_BYTEENDIAN "${CMAKE_CURRENT_SOURCE_DIR}/byteEndianBench.c")
set(BENCH_BYTEINT "${CMAKE_CURRENT_SOURCE_DIR}/byteIntBench.c")

set(BENCH_ASSETS
  "${CMAKE_SOURCE_DIR}/assets/latin1.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf8-complex.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf8-simple.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf16.txt"
  "${CMAKE_SOURCE_DIR}/assets/ascii.txt"
)

add_executable(unicode_bench ${BENCH_BYTEUNICODE})
add_executable(stream_bench ${BENCH_BYTESTREAM})
add_executable(endian_bench ${BENCH_BYTEENDIAN})
add_executable(int_bench ${BENCH_BYTEINT})

set_target_properties(unicode_bench PROPERTIES C_STANDARD 99)
set_target_properties(stream_bench PROPERTIES C_STANDARD 99)
set_target_properties(endian_bench PROPERTIES C_STANDARD 99)
set_target_properties(int_bench PROPERTIES C_STANDARD 99)

target_link_libraries(unicode_bench PRIVATE ByteStream)
target_link_libraries(stream_bench PRIVATE ByteStream)
target_link_libraries(endian_bench PRIVATE ByteStream)
target_link_libraries(int_bench PRIVATE ByteStream)

file(
  COPY ${BENCH_ASSETS}
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets"
)