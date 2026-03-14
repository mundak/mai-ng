include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE GTest::gtest_main)

gtest_discover_tests(test_main)

add_executable(test_window test_window.cpp)
target_include_directories(test_window PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_window PRIVATE GTest::gtest_main imgui spdlog::spdlog)
target_sources(test_window PRIVATE
  ${CMAKE_SOURCE_DIR}/src/app/window.cpp
)

gtest_discover_tests(test_window)
