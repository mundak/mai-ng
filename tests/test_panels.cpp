#include "app/imgui_backend.h"
#include "app/layout.h"
#include "app/window.h"
#include "panels/assembly_panel.h"
#include "panels/console_panel.h"
#include "panels/debugger_panel.h"
#include "panels/editor_panel.h"

#include <gtest/gtest.h>
#include <imgui.h>

class panel_test : public ::testing::Test
{
protected:
  mai::app::window win;
  mai::app::imgui_backend backend;

  void SetUp() override
  {
    ASSERT_TRUE(win.init(1280, 720, "Mai IDE"));
    backend.init(win.handle());
  }

  void TearDown() override
  {
    backend.shutdown();
    win.shutdown();
  }

  void begin_frame() { backend.new_frame(); }

  void end_frame() { ImGui::Render(); }
};

TEST_F(panel_test, editor_panel_renders)
{
  begin_frame();
  mai::panels::editor_panel panel;
  panel.render();
  end_frame();
  SUCCEED();
}

TEST_F(panel_test, assembly_panel_renders)
{
  begin_frame();
  mai::panels::assembly_panel panel;
  panel.render();
  end_frame();
  SUCCEED();
}

TEST_F(panel_test, console_panel_renders)
{
  begin_frame();
  mai::panels::console_panel panel;
  panel.render();
  end_frame();
  SUCCEED();
}

TEST_F(panel_test, debugger_panel_renders)
{
  begin_frame();
  mai::panels::debugger_panel panel;
  panel.render();
  end_frame();
  SUCCEED();
}

TEST_F(panel_test, setup_default_layout_no_crash)
{
  begin_frame();
  ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();
  mai::app::setup_default_layout(dockspace_id);
  end_frame();
  SUCCEED();
}
