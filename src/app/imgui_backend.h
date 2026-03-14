#pragma once

struct GLFWwindow;

namespace mai::app
{

  class imgui_backend
  {
  public:
    void init(GLFWwindow* glfw_window);
    void new_frame();
    void render();
    void shutdown();
  };

}
