# Contributing to Mai IDE

This guide describes coding conventions and the agent workflow for the Mai IDE
project. All contributors — human and AI — must follow these rules.

---

## Build & Test

```bash
# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build --parallel

# Run tests
ctest --test-dir build --output-on-failure

# Run the application
./build/src/mai-ide
```

**Every PR must leave the build and test suite green.**

---

## Language & Standard

* **C++20** (`-std=c++20`). Use modern features (structured bindings, concepts,
  `std::format`, `constexpr`, etc.) where they improve clarity.
* Compile with `-Wall -Wextra -Wpedantic` (GCC/Clang) or `/W4` (MSVC).
  **Zero warnings policy.**

---

## Naming Conventions

| Entity | Style | Example |
|--------|-------|---------|
| Files & directories | `snake_case` | `main_loop.cpp`, `editor_panel.h` |
| Namespaces | `snake_case` | `mai::app`, `mai::panels` |
| Classes / structs | `PascalCase` | `EditorPanel`, `Window` |
| Functions / methods | `snake_case` | `poll_events()`, `new_frame()` |
| Local variables | `snake_case` | `window_width` |
| Member variables | `snake_case_` (trailing underscore) | `title_` |
| Constants / enums | `kPascalCase` or `UPPER_SNAKE` | `kMaxWidth`, `EXIT_SUCCESS` |
| Macros (avoid) | `UPPER_SNAKE` | `MAI_DEBUG` |

---

## File Layout

Each module lives in its own subdirectory under `src/`:

```
src/
├── main.cpp           # entry point
├── app/               # application shell (window, imgui, loop, layout)
├── panels/            # UI panels (editor, assembly, console, debugger)
├── compiler/          # lexer, parser, AST, codegen (future)
└── debugger/          # GDB stub client (future)
```

Headers use `.h`, source files use `.cpp`.

---

## Header Guards

Use `#pragma once` (supported by all target compilers).

---

## Namespaces

All code lives under the `mai` namespace, further scoped by module:

```cpp
namespace mai::app { … }
namespace mai::panels { … }
namespace mai::compiler { … }   // future
namespace mai::debugger { … }   // future
```

---

## Includes

1. Corresponding header (for `.cpp` files).
2. Project headers (alphabetical).
3. Third-party headers (`imgui.h`, `GLFW/glfw3.h`, etc.).
4. Standard library headers.

Separate each group with a blank line.

---

## Testing

* **Framework:** GoogleTest (fetched via CMake `FetchContent`).
* **Location:** `tests/` directory.
* **Naming:** `test_<module>.cpp` → test executable `test_<module>`.
* **Registration:** `gtest_discover_tests(<target>)` in `tests/CMakeLists.txt`.
* **CI:** Tests run under `xvfb-run` when they need an OpenGL context.

---

## Commit Messages

```
<type>(<scope>): <short summary>

<optional body>
```

Types: `feat`, `fix`, `test`, `docs`, `refactor`, `ci`, `chore`.
Scopes: `app`, `panels`, `compiler`, `debugger`, `ci`, `docs`.

Examples:
```
feat(app): add GLFW window creation
test(panels): add render smoke tests for all panels
ci: install Mesa packages for headless GL
```

---

## Pull Requests

* **One task per PR.** Reference the task ID (e.g. `T0.1`, `T1.4`) in the
  PR title.
* Ensure CI is green before requesting review.
* Keep PRs small and focused.

---

## Agent Workflow

AI agents picking up tasks should:

1. Read the task description in `docs/tasks/m<N>-tasks.md`.
2. Check that all prerequisite tasks are merged or present on the branch.
3. Create a feature branch: `agent/<task-id>-<short-name>`
   (e.g. `agent/t0.1-cmake`).
4. Implement the deliverables.
5. Write (or update) the suggested tests.
6. Run `cmake -B build && cmake --build build && ctest --test-dir build`
   locally.
7. Commit with a descriptive message referencing the task ID.
8. Open a PR targeting the milestone integration branch.

---

## Dependencies

* Managed via CMake `FetchContent`.
* New dependencies must be justified and documented.
* Prefer header-only or small, well-maintained libraries.
* Current dependencies:
  - **GLFW 3.4** — windowing / input
  - **Dear ImGui (docking branch)** — GUI framework
  - **GoogleTest 1.14** — testing
