# M2 — Dockable Panel Layout — Task List

**Milestone goal:** Four named, dockable panels arranged in the default layout
described in the TDD.

**Demo:** Launch → four labelled panels visible; drag a panel tab to rearrange.

**Prerequisites:** M1 complete (window + ImGui context + render loop).

> **TDD reference:** §3.1 — Application Shell (layout)

---

## Parallel Groups

```
Group A (4 tasks, all independent — run in parallel)
  T2.1  Editor panel (empty)
  T2.2  Assembly View panel (empty)
  T2.3  Console / Output panel (empty)
  T2.4  Debugger panel (empty)

Group B (2 tasks, depend on all of Group A)
  T2.5  Default dock layout (DockBuilder)
  T2.6  Wire panels and layout into the main loop

Group C (2 tasks, depend on Group B)
  T2.7  Tests for M2
  T2.8  End-to-end verification
```

---

## Tasks

### T2.1 — Editor Panel

**Parallel group:** A
**Depends on:** M1 complete
**Files to create / modify:**
- `src/panels/editor_panel.h`
- `src/panels/editor_panel.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create an empty, dockable ImGui panel labelled "Editor". In M3 this will host
the text-editing widget; for now it simply displays a placeholder label.

**Acceptance criteria:**
- `mai::panels::EditorPanel` class with:
  - `void render()` — calls `ImGui::Begin("Editor")`, renders a placeholder
    text (`"Editor panel — M3 will add text input here."`), calls
    `ImGui::End()`.
- The panel's window name is exactly `"Editor"` (this name will be referenced
  by the DockBuilder layout).
- Header and source file follow project conventions (see contributing.md).
- Compiles with no warnings.

**Suggested tests:**
- (Verified visually in T2.8 and programmatically in T2.7.)

---

### T2.2 — Assembly View Panel

**Parallel group:** A
**Depends on:** M1 complete
**Files to create / modify:**
- `src/panels/assembly_panel.h`
- `src/panels/assembly_panel.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create an empty, dockable ImGui panel labelled "Assembly View". In later
milestones this will show generated ARM64 assembly.

**Acceptance criteria:**
- `mai::panels::AssemblyPanel` class with:
  - `void render()` — `ImGui::Begin("Assembly View")`, placeholder text
    (`"Assembly View panel — M10 will add assembly display here."`),
    `ImGui::End()`.
- Window name is exactly `"Assembly View"`.
- Compiles with no warnings.

**Suggested tests:**
- (Verified in T2.7 and T2.8.)

---

### T2.3 — Console / Output Panel

**Parallel group:** A
**Depends on:** M1 complete
**Files to create / modify:**
- `src/panels/console_panel.h`
- `src/panels/console_panel.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create an empty, dockable ImGui panel labelled "Console". This will later
display compiler errors, program stdout/stderr, and accept stdin.

**Acceptance criteria:**
- `mai::panels::ConsolePanel` class with:
  - `void render()` — `ImGui::Begin("Console")`, placeholder text
    (`"Console panel — M7 will add error/output display here."`),
    `ImGui::End()`.
- Window name is exactly `"Console"`.
- Compiles with no warnings.

**Suggested tests:**
- (Verified in T2.7 and T2.8.)

---

### T2.4 — Debugger Panel

**Parallel group:** A
**Depends on:** M1 complete
**Files to create / modify:**
- `src/panels/debugger_panel.h`
- `src/panels/debugger_panel.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create an empty, dockable ImGui panel labelled "Debugger". This will later
host breakpoints, registers, memory inspection, and watch/locals.

**Acceptance criteria:**
- `mai::panels::DebuggerPanel` class with:
  - `void render()` — `ImGui::Begin("Debugger")`, placeholder text
    (`"Debugger panel — M15 will add debugging controls here."`),
    `ImGui::End()`.
- Window name is exactly `"Debugger"`.
- Compiles with no warnings.

**Suggested tests:**
- (Verified in T2.7 and T2.8.)

---

### T2.5 — Default Dock Layout

**Parallel group:** B
**Depends on:** T2.1, T2.2, T2.3, T2.4
**Files to create / modify:**
- `src/app/layout.h`
- `src/app/layout.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Use the ImGui DockBuilder API to programmatically arrange the four panels into
a default layout on the first launch. The layout matches the TDD §3.1
architecture diagram.

**Target layout:**
```
┌──────────────────┬───────────────────┐
│                  │                   │
│     Editor       │   Assembly View   │
│     (left)       │   (right top)     │
│                  ├───────────────────┤
│                  │   Debugger        │
│                  │   (right bottom)  │
├──────────────────┴───────────────────┤
│          Console / Output            │
│          (bottom)                    │
└──────────────────────────────────────┘
```

**Acceptance criteria:**
- `mai::app::setup_default_layout(ImGuiID dockspace_id)` function that:
  1. Checks whether the layout has already been configured (e.g. using
     `ImGui::DockBuilderGetNode`). If so, returns early (preserves user
     rearrangement).
  2. Clears the dockspace node.
  3. Splits the dockspace:
     - Bottom 25%: "Console"
     - Remaining top-left 55%: "Editor"
     - Top-right upper 60%: "Assembly View"
     - Top-right lower 40%: "Debugger"
  4. Docks each panel window by its name.
  5. Calls `ImGui::DockBuilderFinish`.
- The layout only applies on the *first launch* (ImGui's ini file preserves
  subsequent user changes).

**Suggested tests:**
- Difficult to unit-test DockBuilder in headless mode. Verified visually in
  T2.8. A structural test can verify the function compiles and doesn't crash
  when called with a valid dockspace ID.

---

### T2.6 — Wire Panels and Layout into the Main Loop

**Parallel group:** B
**Depends on:** T2.1, T2.2, T2.3, T2.4, T2.5
**Files to create / modify:**
- `src/app/main_loop.h` / `src/app/main_loop.cpp` (modify from M1)
- `src/main.cpp` (if needed)

**Description:**
Integrate the four panels and the default layout into the main render loop.
Each frame should create a full-viewport dockspace and then render all panels.

**Acceptance criteria:**
- The main loop creates a full-viewport dockspace every frame:
  ```cpp
  ImGuiID dockspace_id = ImGui::DockSpaceOverViewport();
  ```
- On the first frame (or first launch), `setup_default_layout(dockspace_id)` is
  called.
- Each panel's `render()` is called every frame.
- The ImGui demo window call (from M1) is removed or put behind a debug flag.
- The application compiles and runs with four visible, dockable panels.

**Suggested tests:**
- (Tested by T2.7 and T2.8.)

---

### T2.7 — Tests for M2

**Parallel group:** C
**Depends on:** T2.5, T2.6
**Files to create / modify:**
- `tests/test_panels.cpp`
- `tests/CMakeLists.txt` (add new test executable)

**Description:**
Add automated tests for the panel and layout code.

**Acceptance criteria:**
- `test_panels` executable registered with `gtest_discover_tests`.
- At least these tests:
  - `PanelTest.EditorPanelRenders` — instantiate `EditorPanel`, call
    `render()` inside an ImGui frame without crashing.
  - `PanelTest.AssemblyPanelRenders` — same for `AssemblyPanel`.
  - `PanelTest.ConsolePanelRenders` — same for `ConsolePanel`.
  - `PanelTest.DebuggerPanelRenders` — same for `DebuggerPanel`.
  - `LayoutTest.SetupDefaultLayoutNoCrash` — call
    `setup_default_layout(dockspace_id)` with a valid dockspace ID, verify
    no crash.
- Tests can run under `xvfb-run` in CI.
- All existing M0 and M1 tests still pass.

**Suggested tests:**
- (This task *is* the test task.)

---

### T2.8 — End-to-End Verification

**Parallel group:** C
**Depends on:** T2.7 (all prior tasks)
**Files to create / modify:**
- (none — verification only)

**Description:**
Verify the full M2 deliverable works end-to-end.

**Acceptance criteria:**
- `cmake -B build && cmake --build build` succeeds with zero warnings.
- `ctest --test-dir build --output-on-failure` — all tests pass (M0 + M1 + M2).
- Running the binary shows four labelled panels ("Editor", "Assembly View",
  "Console", "Debugger") in the default layout.
- Panels can be dragged, undocked, and re-docked.
- Closing and re-opening the app restores the last layout (ImGui ini
  persistence).
- CI workflow is green.

**Suggested tests:**
- (This task *is* the integration test.)
