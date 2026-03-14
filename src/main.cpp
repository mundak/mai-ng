#include "app/imgui_backend.h"
#include "app/main_loop.h"
#include "app/window.h"
#include "panels/assembly_panel.h"
#include "panels/console_panel.h"
#include "panels/debugger_panel.h"
#include "panels/editor_panel.h"

#include <spdlog/spdlog.h>

#include <cstdlib>

int main()
{
  mai::app::window win;
  mai::app::imgui_backend backend;

  if (!win.init(1280, 720, "Mai IDE"))
  {
    spdlog::error("Failed to initialise window — exiting");
    return EXIT_FAILURE;
  }

  backend.init(win.handle());

  mai::panels::editor_panel editor;
  mai::panels::assembly_panel assembly;
  mai::panels::console_panel console;
  mai::panels::debugger_panel debugger;

  mai::app::run(win, backend, editor, assembly, console, debugger);

  backend.shutdown();
  win.shutdown();

  return EXIT_SUCCESS;
}
