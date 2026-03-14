#include "panels/console_panel.h"

#include <imgui.h>

void console_panel::render()
{
  ImGui::Begin("Console");
  ImGui::TextUnformatted("Console panel — M7 will add error/output display here.");
  ImGui::End();
}
