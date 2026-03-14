#include "panels/editor_panel.h"

#include <imgui.h>

namespace mai::panels
{

  void editor_panel::render()
  {
    ImGui::Begin("Editor");
    ImGui::TextUnformatted("Editor panel \xe2\x80\x94 M3 will add text input here.");
    ImGui::End();
  }

}
