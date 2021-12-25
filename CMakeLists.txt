cmake_minimum_required(VERSION 3.16.3)

# set the project name
project(pathtracer)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_compile_options(
       -Wall -Werror -Wextra -Ofast -Wno-missing-field-initializers
)

# add the executable
add_executable(${PROJECT_NAME} ./src/main.cc)
