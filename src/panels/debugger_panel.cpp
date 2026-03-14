#include "panels/debugger_panel.h"

#include <imgui.h>

void debugger_panel::render()
{
  ImGui::Begin("Debugger");
  ImGui::TextUnformatted("Debugger panel — M15 will add debugging controls here.");
  ImGui::End();
}
