cmake_minimum_required(VERSION 3.22.1)
project(ByteStream VERSION 1.3.0.0)

option(BUILD_SHARED_LIBS "building a shared lib" OFF)
option(BUILD_TESTS "build tests" OFF)
option(BUILD_BENCHMARKS "build benchmarks" OFF)
option(BUILD_DOCS "build documentation" OFF)
option(BUILD_STREAM_DEBUG "compiles with debug symbols" ON)

set(BYTESTREAM_HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteDefines.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteTypes.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteUnicode.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteStream.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteEndian.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/byteInt.h"
)


set(BYTESTREAM_SRC
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteUnicode.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteEndian.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/byteStream.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/byteInt.c"
)

if(BUILD_STREAM_DEBUG)
  if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_compile_options("/W4")
  else()
    add_compile_options("-Wall")
    add_compile_options("-g")
  endif()
endif()


add_library(ByteStream ${BYTESTREAM_HEADERS} ${BYTESTREAM_SRC})
target_include_directories(ByteStream PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include")
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

set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

if(UNIX AND NOT APPLE)
  include(GNUInstallDirs)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr")
  install(TARGETS ByteStream DESTINATION lib)
  install(FILES ${BYTESTREAM_HEADERS} DESTINATION "include/ByteStream")
  
elseif(WIN32)
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "ByteStream")
  set(CPACK_GENERATOR "NSIS")
  set(CPACK_NSIS_PACKAGE_NAME "ByteStream")
  install(TARGETS ByteStream DESTINATION lib)
  install(FILES ${BYTESTREAM_HEADERS} DESTINATION "include/ByteStream")
elseif(APPLE)
#i dont have mac hardware :(
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
include(CPack)
