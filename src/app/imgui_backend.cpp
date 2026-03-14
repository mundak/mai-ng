#include "app/imgui_backend.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>

namespace
{

  constexpr const char* GLSL_VERSION = "#version 330";

}

namespace mai::app
{

  void imgui_backend::init(GLFWwindow* glfw_window)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    io.Fonts->AddFontDefault();

    spdlog::info("ImGui backend initialised (docking enabled, GLSL {})", GLSL_VERSION);
  }

  void imgui_backend::new_frame()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void imgui_backend::render()
  {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void imgui_backend::shutdown()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    spdlog::info("ImGui backend shut down");
  }

}
