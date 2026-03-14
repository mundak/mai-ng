#include "app/layout.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <spdlog/spdlog.h>

namespace mai::app
{

  void setup_default_layout(ImGuiID dockspace_id)
  {
    ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
    if (node != nullptr && node->ChildNodes[0] != nullptr)
    {
      return;
    }

    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

    ImGuiID top_id = dockspace_id;
    ImGuiID bottom_id = ImGui::DockBuilderSplitNode(top_id, ImGuiDir_Down, 0.25f, nullptr, &top_id);
    ImGuiID left_id = top_id;
    ImGuiID right_id = ImGui::DockBuilderSplitNode(left_id, ImGuiDir_Right, 0.45f, nullptr, &left_id);
    ImGuiID right_top_id = right_id;
    ImGuiID right_bottom_id = ImGui::DockBuilderSplitNode(right_top_id, ImGuiDir_Down, 0.40f, nullptr, &right_top_id);

    ImGui::DockBuilderDockWindow("Editor", left_id);
    ImGui::DockBuilderDockWindow("Assembly View", right_top_id);
    ImGui::DockBuilderDockWindow("Debugger", right_bottom_id);
    ImGui::DockBuilderDockWindow("Console", bottom_id);

    ImGui::DockBuilderFinish(dockspace_id);

    spdlog::info("Default dock layout applied");
  }

}
