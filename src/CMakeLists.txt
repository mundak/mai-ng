add_executable(mai-ide
  main.cpp
  app/window.cpp
  app/imgui_backend.cpp
  app/main_loop.cpp
)
target_include_directories(mai-ide PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mai-ide PRIVATE spdlog::spdlog imgui)
