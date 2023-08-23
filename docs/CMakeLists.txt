cmake_minimum_required(VERSION 3.27.1)

find_package(Doxygen)

if(DOXYGEN_FOUND)

    set(DOXYGEN_PROJECT_NAME ${PROJECT_NAME})
    set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    set(DOXYGEN_PROJECT_BRIEF "A stream for binary and unicode")

    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
    set(DOXYGEN_MARKDOWN_SUPPORT YES)

    set(_doxyfile_template "${CMAKE_BINARY_DIR}/CMakeDoxyfile.in")
    set(_target_doxyfile "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.docs")
    configure_file("${_doxyfile_template}" "${_target_doxyfile}")

    doxygen_add_docs(docs
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${BYTESTREAM_HEADERS}
                    ${BYTESTREAM_SRC})


endif()