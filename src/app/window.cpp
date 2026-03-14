#include "app/window.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace mai::app
{

  static void glfw_error_callback(int error, const char* description)
  {
    spdlog::error("GLFW error {}: {}", error, description);
  }

  static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
  {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  }

  bool Window::init(int width, int height, const char* title)
  {
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
      spdlog::error("Failed to initialise GLFW");
      return false;
    }
    m_glfw_initialised = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!m_window)
    {
      spdlog::error("Failed to create GLFW window");
      glfwTerminate();
      return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(m_window, key_callback);

    spdlog::info("Window created: {}x{} — \"{}\"", width, height, title);
    return true;
  }

  bool Window::should_close() const
  {
    return glfwWindowShouldClose(m_window) != 0;
  }

  void Window::poll_events()
  {
    glfwPollEvents();
  }

  void Window::swap_buffers()
  {
    glfwSwapBuffers(m_window);
  }

  void Window::shutdown()
  {
    if (m_window)
    {
      glfwDestroyWindow(m_window);
      m_window = nullptr;
    }
    if (m_glfw_initialised)
    {
      glfwTerminate();
      m_glfw_initialised = false;
    }
    spdlog::info("Window shut down");
  }

} // namespace mai::app
