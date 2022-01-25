cmake_minimum_required(VERSION 3.16.3)

project(pathtracer)

file(GLOB SOURCES
    src/*.cpp
    src/math/*.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(
    -Wall -Werror -Wextra -Wpedantic -Wno-unused-function -Wno-unused-variable -Ofast -fdiagnostics-color=always -march=native -Wno-missing-field-initializers
)

add_executable(${PROJECT_NAME} ${SOURCES})

