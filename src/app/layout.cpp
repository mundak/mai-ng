#include "app/layout.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

namespace mai::app
{

  void setup_default_layout(ImGuiID dockspace_id)
  {
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
    if (node != nullptr && node->IsSplitNode())
    {
      return;
    }

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);

    ImGuiID top_id = 0;
    ImGuiID bottom_id = 0;
    ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, &bottom_id, &top_id);

    ImGuiID left_id = 0;
    ImGuiID right_id = 0;
    ImGui::DockBuilderSplitNode(top_id, ImGuiDir_Left, 0.55f, &left_id, &right_id);

    ImGuiID right_top_id = 0;
    ImGuiID right_bottom_id = 0;
    ImGui::DockBuilderSplitNode(right_id, ImGuiDir_Down, 0.40f, &right_bottom_id, &right_top_id);

    ImGui::DockBuilderDockWindow("Editor", left_id);
    ImGui::DockBuilderDockWindow("Assembly View", right_top_id);
    ImGui::DockBuilderDockWindow("Debugger", right_bottom_id);
    ImGui::DockBuilderDockWindow("Console", bottom_id);

    ImGui::DockBuilderFinish(dockspace_id);

    spdlog::info("Default dock layout applied");
  }

}
