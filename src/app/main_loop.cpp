#include "app/main_loop.h"

#include "app/imgui_backend.h"
#include "app/window.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace mai::app
{

  void run(Window& window, ImGuiBackend& imgui_backend)
  {
    while (!window.should_close())
    {
      window.poll_events();
      imgui_backend.new_frame();

      ImGui::ShowDemoWindow();

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      imgui_backend.render();
      window.swap_buffers();
    }
  }

} // namespace mai::app
