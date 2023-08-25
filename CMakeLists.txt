cmake_minimum_required(VERSION 3.27.1)
project(ByteStream VERSION 1.0.0.0)

option(BUILD_SHARED_LIBS "building a shared lib" ON)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_BENCHMARKS "build benchmarks" OFF)
option(BUILD_DOCS "build documentation" ON)

set(BYTESTREAM_HEADERS
  "${CMAKE_SOURCE_DIR}/include/byteDefines.h"
  "${CMAKE_SOURCE_DIR}/include/byteTypes.h"
  "${CMAKE_SOURCE_DIR}/include/byteUnicode.h"
  "${CMAKE_SOURCE_DIR}/include/byteStream.h"
  "${CMAKE_SOURCE_DIR}/include/byteEndian.h"
  "${CMAKE_SOURCE_DIR}/include/byteInt.h"
)


set(BYTESTREAM_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteUnicode.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteEndian.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/byteStream.c"
  "${CMAKE_SOURCE_DIR}/src/modules/byteInt.c"
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

if(BUILD_DOCS)
  add_subdirectory(docs)
endif()

#install on linux
if(UNIX AND NOT APPLE)
  include(GNUInstallDirs)
  install(TARGETS ByteStream DESTINATION "/usr/lib")
  install(FILES ${BYTESTREAM_HEADERS} DESTINATION "/usr/include/ByteStream")

  include(InstallRequiredSystemLibraries)
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
  include(CPack)
elseif(WIN32)
#i dont know how to do this :(
elseif(APPLE)
#i dont have mac hardware :(
endif()

