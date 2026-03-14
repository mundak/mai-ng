#include "app/imgui_backend.h"
#include "app/main_loop.h"
#include "app/window.h"

#include <spdlog/spdlog.h>

#include <cstdlib>

int main()
{
  mai::app::window win;
  mai::app::imgui_backend backend;

  if (!win.init(1280, 720, "Mai IDE"))
  {
    spdlog::error("Failed to initialise window — exiting");
    return EXIT_FAILURE;
  }

  backend.init(win.handle());
  mai::app::run(win, backend);

  backend.shutdown();
  win.shutdown();

  return EXIT_SUCCESS;
}
