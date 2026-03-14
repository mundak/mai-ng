cmake_minimum_required(VERSION 3.20)
project(mai-ide VERSION 0.1.0 LANGUAGES CXX)

# ── C++20 ────────────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Tooling support ──────────────────────────────────────────────────────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Dependencies ─────────────────────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.15.0
)
FetchContent_MakeAvailable(spdlog)

# ── Source ───────────────────────────────────────────────────────────────────
add_subdirectory(src)

# ── Tests ────────────────────────────────────────────────────────────────────
option(MAI_BUILD_TESTS "Build tests" ON)
if(MAI_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
