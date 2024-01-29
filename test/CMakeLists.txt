cmake_minimum_required(VERSION 3.22.1)

# find_library(CMOCKA_LIBRARIES REQUIRED NAMES cmocka)
# if(CMOCKA_LIBRARIES)
#     message(STATUS "cmocka library found: ${CMOCKA_LIBRARIES}")
# else()
#     #cmocka not found
#     message(FATAL_ERROR "cmocka library not found please install cmocka to run these tests")
# endif()

set(TEST_BYTESTREAM "${CMAKE_CURRENT_SOURCE_DIR}/byteStreamFunctions.c")
set(TEST_BYTEUNICODE "${CMAKE_CURRENT_SOURCE_DIR}/byteUnicodeFunctions.c")
set(TEST_BYTEENDIAN "${CMAKE_CURRENT_SOURCE_DIR}/byteEndianFunctions.c")
set(TEST_BYTEINT "${CMAKE_CURRENT_SOURCE_DIR}/byteIntFunctions.c")

set(TEST_ASSETS
  "${CMAKE_CURRENT_SOURCE_DIR}/../assets/latin1.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/../assets/utf8-complex.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/../assets/utf8-simple.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/../assets/utf16.txt"
  "${CMAKE_CURRENT_SOURCE_DIR}/../assets/ascii.txt"
)

add_executable(unicode_test ${TEST_BYTEUNICODE})
add_executable(stream_test ${TEST_BYTESTREAM})
add_executable(endian_test ${TEST_BYTEENDIAN})
add_executable(int_test ${TEST_BYTEINT})

set_target_properties(unicode_test PROPERTIES C_STANDARD 99)
set_target_properties(stream_test PROPERTIES C_STANDARD 99)
set_target_properties(endian_test PROPERTIES C_STANDARD 99)
set_target_properties(int_test PROPERTIES C_STANDARD 99)

target_link_libraries(unicode_test PRIVATE ByteStream)
target_link_libraries(unicode_test PRIVATE cmocka)

target_link_libraries(stream_test PRIVATE ByteStream)
target_link_libraries(stream_test PRIVATE cmocka)

target_link_libraries(endian_test PRIVATE ByteStream)
target_link_libraries(endian_test PRIVATE cmocka)

target_link_libraries(int_test PRIVATE ByteStream)
target_link_libraries(int_test PRIVATE cmocka)


file(
  COPY ${TEST_ASSETS}
  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets"
)