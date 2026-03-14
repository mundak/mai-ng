#include "app/main_loop.h"

#include "app/imgui_backend.h"
#include "app/window.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace mai::app
{

  void run(window& win, imgui_backend& backend)
  {
    while (!win.should_close())
    {
      win.poll_events();
      backend.new_frame();

      ImGui::ShowDemoWindow();

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      backend.render();
      win.swap_buffers();
    }
  }

}
