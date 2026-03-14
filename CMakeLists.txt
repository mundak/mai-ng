cmake_minimum_required(VERSION 3.20)

project(
  mai-ide
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Testing
option(MAI_BUILD_TESTS "Build unit tests" ON)

if(MAI_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
