cmake_minimum_required(VERSION 3.12)
project(mfront-jit LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(TFELMFront REQUIRED HINTS "${TFEL_DIR}/share/tfel/cmake")

add_subdirectory(src)
