#include "panels/editor_panel.h"

#include <imgui.h>

void editor_panel::render()
{
  ImGui::Begin("Editor");
  ImGui::TextUnformatted("Editor panel \u2014 M3 will add text input here.");
  ImGui::End();
}
