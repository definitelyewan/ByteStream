cmake_minimum_required(VERSION 3.27.1)

find_library(CMOCKA_LIBRARIES REQUIRED NAMES cmocka)
if(CMOCKA_LIBRARIES)
    message(STATUS "cmocka library found: ${CMOCKA_LIBRARIES}")
else()
    #cmocka not found
    message(FATAL_ERROR "cmocka library not found please install cmocka to run these tests")
endif()

set(TEST_BYTESTREAM "${CMAKE_CURRENT_SOURCE_DIR}/byteStreamFunctions.c")
set(TEST_BYTEUNICODE "${CMAKE_CURRENT_SOURCE_DIR}/byteUnicodeFunctions.c")

set(TEST_ASSETS
  "${CMAKE_SOURCE_DIR}/assets/latin1.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf8-complex.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf8-simple.txt"
  "${CMAKE_SOURCE_DIR}/assets/utf16.txt"
  "${CMAKE_SOURCE_DIR}/assets/ascii.txt"
)

add_executable(unicode_test ${TEST_BYTEUNICODE})
add_executable(stream_test ${TEST_BYTESTREAM})

set_target_properties(unicode_test PROPERTIES C_STANDARD 99)
set_target_properties(stream_test PROPERTIES C_STANDARD 99)

target_link_libraries(unicode_test PRIVATE ByteStream)
target_link_libraries(stream_test PRIVATE ByteStream)
target_link_libraries(unicode_test PRIVATE cmocka)
target_link_libraries(stream_test PRIVATE cmocka)

file(
  COPY ${TEST_ASSETS}
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets"
)