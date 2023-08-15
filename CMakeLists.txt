cmake_minimum_required(VERSION 3.27.1)
project(ByteStream VERSION 1.0.0.0)

option(BUILD_SHARED_LIBS "building a shared lib" OFF)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_BENCHMARKS "build benchmarks" OFF)

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

#install section fingers crossed this works on windows too
install(TARGETS ByteStream DESTINATION lib)
install(FILES ${BYTESTREAM_HEADERS} DESTINATION include)

if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  include(InstallRequiredSystemLibraries)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  include(CPack)
endif()
