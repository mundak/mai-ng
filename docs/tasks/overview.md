# Task Lists — Overview

This directory contains **granular, agent-ready task lists** for the first three
milestones of the Mai IDE project. Each task is designed to be picked up by an
independent AI coding agent (or human contributor) and completed in isolation.

> **Reference documents:**
> [Technical Design Overview](../tech-design.md) ·
> [Milestones](../milestones.md) ·
> [Contributing Guide](../contributing.md)

---

## Covered Milestones

| File | Milestone | Summary |
|------|-----------|---------|
| [m0-tasks.md](m0-tasks.md) | M0 — Project Skeleton & CI | CMake, main.cpp, GoogleTest, GitHub Actions |
| [m1-tasks.md](m1-tasks.md) | M1 — Empty Window on Screen | GLFW, Dear ImGui, OpenGL, render loop |
| [m2-tasks.md](m2-tasks.md) | M2 — Dockable Panel Layout | Four panels, DockBuilder, default layout |

---

## Milestone Dependency Chain

```
M0 ──► M1 ──► M2
```

Each milestone must be **fully complete** (CI green) before the next one
begins. Within a single milestone, tasks in the same **parallel group** have no
dependencies on each other and can be worked simultaneously by separate agents.

---

## How to Read a Task File

Every task file uses the following structure:

```
### T<M>.<N> — <Short title>

**Parallel group:** <letter>          ← tasks sharing a letter can run concurrently
**Depends on:** <list or "none">      ← prerequisite tasks within this milestone
**Files to create / modify:** …       ← expected deliverables
**Acceptance criteria:**              ← definition of done
  - bullet list …
**Suggested tests:**                  ← tests the agent should write alongside the code
  - bullet list …
```

An agent should:

1. Read the task description and its dependencies.
2. Verify that all prerequisite tasks are merged or present on the working branch.
3. Implement the deliverables.
4. Write the suggested tests (at minimum).
5. Ensure the full project builds and all tests pass (`cmake -B build && cmake --build build && ctest --test-dir build`).
6. Open a PR referencing the task ID (e.g. `T0.1`).

---

## Parallelisation Strategy

```
M0  Group A ──► Group B ──► Group C
    (T0.1–T0.5)  (T0.6)     (T0.7)

M1  Group A ──► Group B ──► Group C ──► Group D
    (T1.1–T1.3)  (T1.4–T1.5) (T1.6–T1.7) (T1.8–T1.9)

M2  Group A ──────────────► Group B ──► Group C
    (T2.1–T2.4)              (T2.5–T2.6) (T2.7–T2.8)
```

**Maximum concurrency per milestone:**

| Milestone | Max parallel agents |
|-----------|---------------------|
| M0 | 5 (group A) |
| M1 | 3 (group A) |
| M2 | 4 (group A) |

---

## Conventions

All agents must follow the conventions in the [Contributing Guide](../contributing.md).
Key rules:

* C++20, `-Wall -Wextra -Wpedantic`
* `snake_case` for files and directories, `PascalCase` for types, `snake_case`
  for functions/variables.
* GoogleTest for all unit tests. Each test executable is registered via
  `gtest_discover_tests()`.
* Every PR must leave `cmake --build build && ctest --test-dir build` green.

---

## Target Directory Layout (After M2)

```
mai-ng/
├── CMakeLists.txt                  # root — project-level settings
├── src/
│   ├── CMakeLists.txt              # src target (mai-ide executable)
│   ├── main.cpp                    # entry point
│   ├── app/
│   │   ├── window.h / .cpp         # GLFW window (M1)
│   │   ├── imgui_backend.h / .cpp  # Dear ImGui init (M1)
│   │   ├── main_loop.h / .cpp      # render loop (M1)
│   │   └── layout.h / .cpp         # dock layout (M2)
│   └── panels/
│       ├── editor_panel.h / .cpp   # (M2)
│       ├── assembly_panel.h / .cpp # (M2)
│       ├── console_panel.h / .cpp  # (M2)
│       └── debugger_panel.h / .cpp # (M2)
├── tests/
│   ├── CMakeLists.txt              # test targets
│   ├── test_main.cpp               # smoke test (M0)
│   ├── test_window.cpp             # (M1)
│   └── test_panels.cpp             # (M2)
├── third_party/                    # vendored or FetchContent deps
├── .github/
│   ├── workflows/ci.yml            # CI pipeline
│   └── ISSUE_TEMPLATE/
│       └── agent-task.yml          # task issue template
├── docs/
│   ├── tech-design.md
│   ├── milestones.md
│   ├── contributing.md
│   └── tasks/
│       ├── overview.md             ← you are here
│       ├── m0-tasks.md
│       ├── m1-tasks.md
│       └── m2-tasks.md
├── .gitignore
└── README.md
```
