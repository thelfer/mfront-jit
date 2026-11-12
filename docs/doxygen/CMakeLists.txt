find_package(Doxygen REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
               ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

# The documentation is never built by a plain build, as the tests of `TFEL`
# are not: it is generated on demand with
#
#   cmake --build <build-dir> --target doc
#
# which writes the `HTML` pages in `<build-dir>/docs/doxygen/html`.
add_custom_target(doc
                  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  COMMENT "Generating the API documentation with doxygen"
                  VERBATIM)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION share/doc/mfront-jit
        COMPONENT doxygen
        OPTIONAL)
