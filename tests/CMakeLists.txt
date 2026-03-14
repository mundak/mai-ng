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

add_executable(test_panels test_panels.cpp)
target_include_directories(test_panels PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_panels PRIVATE GTest::gtest_main imgui spdlog::spdlog)
target_sources(test_panels PRIVATE
  ${CMAKE_SOURCE_DIR}/src/app/window.cpp
  ${CMAKE_SOURCE_DIR}/src/app/imgui_backend.cpp
  ${CMAKE_SOURCE_DIR}/src/app/layout.cpp
  ${CMAKE_SOURCE_DIR}/src/panels/editor_panel.cpp
  ${CMAKE_SOURCE_DIR}/src/panels/assembly_panel.cpp
  ${CMAKE_SOURCE_DIR}/src/panels/console_panel.cpp
  ${CMAKE_SOURCE_DIR}/src/panels/debugger_panel.cpp
)

gtest_discover_tests(test_panels)
