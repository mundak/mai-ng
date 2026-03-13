# M1 — Empty Window on Screen — Task List

**Milestone goal:** Open an OS window with a Dear ImGui context; nothing else
drawn yet.

**Demo:** Launch the binary → a blank window titled *"Mai IDE"* appears.
ESC or window-close exits cleanly.

**Prerequisites:** M0 complete (project skeleton, CI green).

> **TDD reference:** §3.1 — Application Shell

---

## Parallel Groups

```
Group A (3 tasks, all independent — run in parallel)
  T1.1  Add GLFW dependency
  T1.2  Add Dear ImGui (docking branch) dependency
  T1.3  Update CI for OpenGL / headless rendering

Group B (2 tasks, depend on T1.1 + T1.2)
  T1.4  Window creation module (GLFW + OpenGL context)
  T1.5  Dear ImGui backend module (init / shutdown / frame helpers)

Group C (2 tasks, depend on T1.4 + T1.5)
  T1.6  Main render loop
  T1.7  Update src/main.cpp to launch the window

Group D (2 tasks, depend on Group C)
  T1.8  Unit / integration tests for M1
  T1.9  End-to-end verification
```

---

## Tasks

### T1.1 — Add GLFW Dependency

**Parallel group:** A
**Depends on:** M0 complete
**Files to create / modify:**
- `CMakeLists.txt` (root — add FetchContent or find_package)
- `src/CMakeLists.txt` (link GLFW to mai-ide)

**Description:**
Integrate GLFW 3.4+ into the CMake build so it is automatically downloaded
and built. The `mai-ide` target must link against `glfw`.

**Acceptance criteria:**
- GLFW is declared via `FetchContent_Declare` (repo:
  `https://github.com/glfw/glfw.git`, tag: `3.4`).
- `GLFW_BUILD_DOCS`, `GLFW_BUILD_TESTS`, `GLFW_BUILD_EXAMPLES` are set `OFF`.
- `src/CMakeLists.txt` links `mai-ide` to `glfw`.
- The project still configures and builds successfully (even though GLFW is not
  yet used in code — no linker errors).

**Suggested tests:**
- (Build verification only — no runtime test yet.)

---

### T1.2 — Add Dear ImGui (Docking Branch) Dependency

**Parallel group:** A
**Depends on:** M0 complete
**Files to create / modify:**
- `CMakeLists.txt` (root — FetchContent)
- `src/CMakeLists.txt` (define imgui library target, link to mai-ide)

**Description:**
Integrate the Dear ImGui **docking branch** into the build. Dear ImGui does
not have a native CMake build, so this task must create a CMake interface
library that compiles the necessary ImGui sources.

**Acceptance criteria:**
- Dear ImGui docking branch is fetched via `FetchContent_Declare` (repo:
  `https://github.com/ocornut/imgui.git`, branch: `docking`).
- A CMake library target `imgui` is defined that compiles at minimum:
  - `imgui.cpp`, `imgui_demo.cpp`, `imgui_draw.cpp`, `imgui_tables.cpp`,
    `imgui_widgets.cpp`
  - Backend files: `backends/imgui_impl_glfw.cpp`,
    `backends/imgui_impl_opengl3.cpp`
- Include directories are set so `#include "imgui.h"` and
  `#include "backends/imgui_impl_glfw.h"` work.
- `mai-ide` links to `imgui`.
- The project configures and builds with no errors.

**Suggested tests:**
- (Build verification only.)

---

### T1.3 — Update CI for OpenGL / Headless Rendering

**Parallel group:** A
**Depends on:** M0 complete (T0.5 specifically)
**Files to create / modify:**
- `.github/workflows/ci.yml`

**Description:**
The CI runner needs OpenGL headers, Mesa, and X11/Wayland libraries for GLFW
and ImGui to compile. Update the workflow to install the necessary system
packages. Also set up `xvfb` so the binary can be launched headlessly in
future tests.

**Acceptance criteria:**
- CI installs at least:
  `libgl-dev libglu1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev
   libxcursor-dev libxi-dev xvfb`
- Build step continues to succeed after adding GLFW and ImGui deps.
- A comment in the workflow explains why each package group is needed.

**Suggested tests:**
- (CI pipeline itself is the test.)

---

### T1.4 — Window Creation Module

**Parallel group:** B
**Depends on:** T1.1, T1.2
**Files to create / modify:**
- `src/app/window.h`
- `src/app/window.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create a module that initialises GLFW, creates a window with an OpenGL 3.3
core-profile context, and provides a clean shutdown function.

**Acceptance criteria:**
- `mai::app::Window` class (or free functions in `mai::app` namespace) with:
  - `bool init(int width, int height, const char* title)` — creates window,
    sets GL context current, loads GL functions.
  - `bool should_close()` — wraps `glfwWindowShouldClose`.
  - `void poll_events()` — wraps `glfwPollEvents`.
  - `void swap_buffers()` — wraps `glfwSwapBuffers`.
  - `void shutdown()` — destroys window, calls `glfwTerminate`.
- GLFW error callback is set and logs to stderr.
- Default window size: 1280×720.
- Window title: `"Mai IDE"`.
- ESC key sets `glfwSetWindowShouldClose(window, true)`.

**Suggested tests:**
- `test_window.cpp`: Test that `init()` + `shutdown()` succeeds without
  crashing (may require `xvfb-run` in CI or a `--headless` guard).
- Test that `should_close()` returns `false` immediately after init.

---

### T1.5 — Dear ImGui Backend Module

**Parallel group:** B
**Depends on:** T1.1, T1.2
**Files to create / modify:**
- `src/app/imgui_backend.h`
- `src/app/imgui_backend.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create a module that initialises and shuts down the Dear ImGui context with
GLFW + OpenGL3 backends.

**Acceptance criteria:**
- `mai::app::ImGuiBackend` class (or free functions) with:
  - `void init(GLFWwindow* window)` — creates ImGui context, enables docking,
    sets dark theme, initialises GLFW and OpenGL3 backends, loads default font.
  - `void new_frame()` — calls the three `NewFrame()` functions
    (OpenGL3, GLFW, ImGui).
  - `void render()` — calls `ImGui::Render()` and
    `ImGui_ImplOpenGL3_RenderDrawData()`.
  - `void shutdown()` — shuts down backends and destroys ImGui context.
- Docking is enabled: `io.ConfigFlags |= ImGuiConfigFlags_DockingEnable`.
- GLSL version string is `"#version 330"` (matching OpenGL 3.3 core).

**Suggested tests:**
- `test_imgui_backend.cpp`: Test that `init()` + `shutdown()` cycle completes
  without errors (requires a valid GL context, so pair with Window init).

---

### T1.6 — Main Render Loop

**Parallel group:** C
**Depends on:** T1.4, T1.5
**Files to create / modify:**
- `src/app/main_loop.h`
- `src/app/main_loop.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create the application's main render loop that ties the window and ImGui
backend together.

**Acceptance criteria:**
- `mai::app::run()` function that:
  1. Calls `window.poll_events()`.
  2. Calls `imgui_backend.new_frame()`.
  3. Optionally calls `ImGui::ShowDemoWindow()` (useful for debugging; can be
     gated behind a flag or removed in M2).
  4. Calls `glClear(GL_COLOR_BUFFER_BIT)` with a dark background colour.
  5. Calls `imgui_backend.render()`.
  6. Calls `window.swap_buffers()`.
  7. Repeats until `window.should_close()` is true.
- The loop runs at the display's refresh rate (vsync via `glfwSwapInterval(1)`).
- Exits cleanly with no resource leaks.

**Suggested tests:**
- Test that `run()` exits immediately when `should_close()` is forced to true
  (mock/stub or set the flag before calling run).

---

### T1.7 — Update main.cpp to Launch Window

**Parallel group:** C
**Depends on:** T1.4, T1.5, T1.6
**Files to create / modify:**
- `src/main.cpp`

**Description:**
Replace the "print and exit" main with one that initialises the window,
ImGui backend, and enters the main loop.

**Acceptance criteria:**
- `main()` calls `Window::init()`, `ImGuiBackend::init()`, `run()`,
  then shuts down in reverse order.
- If `Window::init()` fails, prints an error to stderr and returns
  `EXIT_FAILURE`.
- The old `"Mai IDE"` print is removed (the window title serves the same
  purpose).
- The application compiles, links, and runs.

**Suggested tests:**
- (Tested by T1.9 end-to-end verification.)

---

### T1.8 — Unit / Integration Tests for M1

**Parallel group:** D
**Depends on:** T1.6, T1.7
**Files to create / modify:**
- `tests/test_window.cpp`
- `tests/CMakeLists.txt` (add new test executable)

**Description:**
Add automated tests for the window and ImGui initialisation. These tests
must be runnable in CI under `xvfb-run`.

**Acceptance criteria:**
- `test_window` executable registered with `gtest_discover_tests`.
- At least these tests:
  - `WindowTest.InitAndShutdown` — init + immediate shutdown, no crash.
  - `WindowTest.DefaultTitle` — verify window title is `"Mai IDE"` via
    `glfwGetWindowTitle`.
  - `WindowTest.ShouldCloseInitiallyFalse` — `should_close()` returns false.
- CI runs the test under `xvfb-run` or equivalent.
- All existing smoke tests still pass.

**Suggested tests:**
- (This task *is* the test task.)

---

### T1.9 — End-to-End Verification

**Parallel group:** D
**Depends on:** T1.8 (all prior tasks)
**Files to create / modify:**
- (none — verification only)

**Description:**
Verify the full M1 deliverable works end-to-end.

**Acceptance criteria:**
- `cmake -B build && cmake --build build` succeeds with zero errors.
- `ctest --test-dir build --output-on-failure` — all tests pass.
- Running `./build/src/mai-ide` opens a window titled "Mai IDE" with a dark
  background. Pressing ESC closes it cleanly.
- In CI, `xvfb-run ./build/src/mai-ide &` starts without errors (kill after
  2 seconds).
- No memory leaks (run under `valgrind` or AddressSanitizer if available).
- CI workflow is green.

**Suggested tests:**
- (This task *is* the integration test.)
