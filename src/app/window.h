#pragma once

#include <cstdint>

struct GLFWwindow;

namespace mai::app
{

  class window
  {
  public:
    bool init(int32_t width, int32_t height, const char* title);
    bool should_close() const;
    void poll_events();
    void swap_buffers();
    void shutdown();

    GLFWwindow* handle() const;

  private:
    GLFWwindow* m_window = nullptr;
    bool m_glfw_initialised = false;
  };

}
