#pragma once

struct GLFWwindow;

namespace mai::app
{

  class ImGuiBackend
  {
  public:
    void init(GLFWwindow* window);
    void new_frame();
    void render();
    void shutdown();
  };

} // namespace mai::app
