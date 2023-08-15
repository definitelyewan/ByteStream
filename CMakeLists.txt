cmake_minimum_required(VERSION 3.27.1)
project(ByteStream VERSION 1.0.0.0)

option(BUILD_SHARED_LIBS "building a shared lib" ON)
option(BUILD_TESTS "build tests" ON)
option(BUILD_BENCHMARKS "build benchmarks" ON)

set(BYTESTREAM_HEADERS
  "${CMAKE_SOURCE_DIR}/include/byteDefines.h"
  "${CMAKE_SOURCE_DIR}/include/byteTypes.h"
  "${CMAKE_SOURCE_DIR}/include/byteUnicode.h"
  "${CMAKE_SOURCE_DIR}/include/byteStream.h"
)


set(BYTESTREAM_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteUnicode.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/byteStream.c"
)

add_library(ByteStream ${BYTESTREAM_HEADERS} ${BYTESTREAM_SRC})
target_include_directories(ByteStream PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}/include")
set_target_properties(ByteStream PROPERTIES C_STANDARD 99)

if(BUILD_TESTS)
    add_subdirectory(test)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

