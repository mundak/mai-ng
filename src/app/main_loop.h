#pragma once

namespace mai::panels
{

  class editor_panel;
  class assembly_panel;
  class console_panel;
  class debugger_panel;

}

namespace mai::app
{

  class window;
  class imgui_backend;

  void run(
    window& win,
    imgui_backend& backend,
    mai::panels::editor_panel& editor,
    mai::panels::assembly_panel& assembly,
    mai::panels::console_panel& console,
    mai::panels::debugger_panel& debugger
  );

}
