cmake_minimum_required(VERSION 3.16.3)

# set the project name
project(pathtracer)

file(GLOB SOURCES
    src/*.cpp
    src/math/*.cpp
    libs/*.cpp
)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(
    -Wall -Werror -Wextra -Wpedantic -Ofast -fdiagnostics-color=always -march=native -Wno-missing-field-initializers
)

add_executable(${PROJECT_NAME} ${SOURCES})

