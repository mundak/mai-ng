#pragma once

struct GLFWwindow;

namespace mai::app
{

  class Window
  {
  public:
    bool init(int width, int height, const char* title);
    bool should_close() const;
    void poll_events();
    void swap_buffers();
    void shutdown();

    GLFWwindow* handle() const { return m_window; }

  private:
    GLFWwindow* m_window = nullptr;
    bool m_glfw_initialised = false;
  };

} // namespace mai::app
