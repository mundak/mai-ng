#include "panels/assembly_panel.h"

#include <imgui.h>

void assembly_panel::render()
{
  ImGui::Begin("Assembly View");
  ImGui::TextUnformatted("Assembly View panel — M10 will add assembly display here.");
  ImGui::End();
}
