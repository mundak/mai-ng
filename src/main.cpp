#include "app/imgui_backend.h"
#include "app/main_loop.h"
#include "app/window.h"

#include <cstdlib>
#include <spdlog/spdlog.h>

int main()
{
  mai::app::Window window;
  mai::app::ImGuiBackend imgui_backend;

  if (!window.init(1280, 720, "Mai IDE"))
  {
    spdlog::error("Failed to initialise window — exiting");
    return EXIT_FAILURE;
  }

  imgui_backend.init(window.handle());
  mai::app::run(window, imgui_backend);

  imgui_backend.shutdown();
  window.shutdown();

  return EXIT_SUCCESS;
}
