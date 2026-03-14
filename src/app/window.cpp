#include "app/window.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace
{

  void glfw_error_callback(int error, const char* description)
  {
    spdlog::error("GLFW error {}: {}", error, description);
  }

  void key_callback(GLFWwindow* glfw_window, int key, int /*scancode*/, int action, int /*mods*/)
  {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(glfw_window, GLFW_TRUE);
    }
  }

}

namespace mai::app
{

  bool window::init(int32_t width, int32_t height, const char* title)
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

  bool window::should_close() const
  {
    return glfwWindowShouldClose(m_window) != 0;
  }

  void window::poll_events()
  {
    glfwPollEvents();
  }

  void window::swap_buffers()
  {
    glfwSwapBuffers(m_window);
  }

  void window::shutdown()
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

  GLFWwindow* window::handle() const
  {
    return m_window;
  }

}
