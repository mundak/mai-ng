#include "app/main_loop.h"

#include "app/imgui_backend.h"
#include "app/layout.h"
#include "app/window.h"
#include "panels/assembly_panel.h"
#include "panels/console_panel.h"
#include "panels/debugger_panel.h"
#include "panels/editor_panel.h"

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace mai::app
{

  void run(
    window& win,
    imgui_backend& backend,
    mai::panels::editor_panel& editor,
    mai::panels::assembly_panel& assembly,
    mai::panels::console_panel& console,
    mai::panels::debugger_panel& debugger
  )
  {
    while (!win.should_close())
    {
      win.poll_events();
      backend.new_frame();

      ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();
      setup_default_layout(dockspace_id);

      editor.render();
      assembly.render();
      console.render();
      debugger.render();

      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      backend.render();
      win.swap_buffers();
    }
  }

}
