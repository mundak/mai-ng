# M3 — Text Input in Editor Panel — Task List

**Milestone goal:** The user can type, select, copy and paste text inside the
Editor panel.

**Demo:** Launch → click the Editor panel → type code → select text → paste
it elsewhere in the buffer.

**Prerequisites:** M2 complete (four panels in dock layout).

> **TDD reference:** §3.2 — Editor Module (text buffer, cursor, selection)

---

## Parallel Groups

```
Group A (2 tasks, independent — run in parallel)
  T3.1  Text buffer class
  T3.2  Add imgui_stdlib to the build

Group B (1 task, depends on all of Group A)
  T3.3  Integrate multi-line text input into editor panel

Group C (2 tasks, depend on Group B)
  T3.4  Unit tests for M3
  T3.5  End-to-end verification
```

---

## Tasks

### T3.1 — Text Buffer Class

**Parallel group:** A
**Depends on:** M2 complete
**Files to create / modify:**
- `src/editor/text_buffer.h`
- `src/editor/text_buffer.cpp`
- `src/CMakeLists.txt` (add new sources)

**Description:**
Create a class that owns the source-text content for the editor. This is the
data model that the editor panel renders and that future milestones will
consume (M5 for lexing, M7 for error display, M24 for file I/O). The class
wraps a `std::string` and provides a clean interface for reading, writing,
and querying the buffer.

The class also tracks a *modified* flag so that downstream consumers can
detect edits. The flag is set whenever the content changes and can be
cleared explicitly (e.g. after a save operation in M24).

**Acceptance criteria:**
- `text_buffer` class (no namespace) in `src/editor/text_buffer.h`.
- Public interface includes at minimum:
  - `std::string& content()` — mutable reference used by
    `ImGui::InputTextMultiline` (via imgui_stdlib).
  - `const std::string& content() const` — read-only access for the compiler
    pipeline.
  - `void set_content(const std::string& text)` — replaces the entire buffer.
  - `void clear()` — empties the buffer.
  - `bool is_empty() const` — returns `true` when the buffer has no content.
  - `int32_t size() const` — returns the number of bytes in the buffer.
  - `bool is_modified() const` — returns the modified flag.
  - `void set_modified(bool value)` — sets or clears the modified flag.
- `set_content()` and `clear()` set the modified flag to `true`.
- The `src/editor/` directory is created.
- `src/CMakeLists.txt` includes the new source files.
- The project compiles with no warnings.

**Suggested tests:**
- `TextBuffer.DefaultEmpty` — default-constructed buffer is empty, size 0,
  not modified.
- `TextBuffer.SetContent` — `set_content("hello")` → `content()` returns
  `"hello"`, `size()` returns 5, `is_modified()` returns true.
- `TextBuffer.Clear` — after `set_content("x")` + `clear()`, buffer is empty
  and modified.
- `TextBuffer.ModifiedFlag` — flag starts false, becomes true on
  `set_content()`, can be reset with `set_modified(false)`.

---

### T3.2 — Add imgui_stdlib to the Build

**Parallel group:** A
**Depends on:** M2 complete
**Files to create / modify:**
- `CMakeLists.txt` (root — add `imgui_stdlib.cpp` to the imgui library target)

**Description:**
Dear ImGui ships with `misc/cpp/imgui_stdlib.cpp` which provides
`ImGui::InputText` and `ImGui::InputTextMultiline` overloads that accept
`std::string*` instead of raw `char*` buffers. This eliminates the need for
manual buffer management and resize callbacks.

Add this source file to the existing `imgui` CMake library target so the
`std::string` overloads are available to the rest of the project.

**Acceptance criteria:**
- `${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp` is added to the `imgui`
  target's source list.
- `${imgui_SOURCE_DIR}/misc/cpp` is added to the include directories (or the
  existing `${imgui_SOURCE_DIR}` include is sufficient to reach
  `misc/cpp/imgui_stdlib.h`).
- A source file can `#include "misc/cpp/imgui_stdlib.h"` and call
  `ImGui::InputTextMultiline(label, &std_string)` without linker errors.
- The project builds with no warnings.
- All existing tests still pass.

**Suggested tests:**
- (Build verification only — no runtime test yet.)

---

### T3.3 — Integrate Multi-Line Text Input into Editor Panel

**Parallel group:** B
**Depends on:** T3.1, T3.2
**Files to create / modify:**
- `src/panels/editor_panel.h`
- `src/panels/editor_panel.cpp`

**Description:**
Replace the placeholder text in the editor panel with a full-size
`ImGui::InputTextMultiline` widget backed by a `text_buffer` instance. The
widget should fill the entire available panel area and support Tab input for
code editing.

After this task, the user can click into the Editor panel and type, move the
cursor with arrow keys, select text with Shift+arrows, and copy/paste with
Ctrl+C / Ctrl+V. All of these features are provided natively by Dear ImGui's
`InputTextMultiline` and the GLFW clipboard backend.

**Acceptance criteria:**
- `editor_panel` holds a `text_buffer` member (e.g. `m_buffer`).
- `render()` replaces the placeholder `TextUnformatted` call with
  `ImGui::InputTextMultiline`:
  ```cpp
  ImGui::InputTextMultiline("##source",
                            &m_buffer.content(),
                            ImVec2(-FLT_MIN, -FLT_MIN),
                            ImGuiInputTextFlags_AllowTabInput);
  ```
- The widget fills the available panel area (using `ImVec2(-FLT_MIN, -FLT_MIN)`
  or equivalent).
- `ImGuiInputTextFlags_AllowTabInput` is set so the Tab key inserts a tab
  character instead of moving focus.
- `editor_panel` exposes a `const text_buffer& buffer() const` accessor so
  future milestones can read the buffer content.
- Typing text into the editor updates `m_buffer.content()`.
- Cursor movement (arrow keys, Home, End), selection (Shift+arrows), and
  clipboard operations (Ctrl+C, Ctrl+V, Ctrl+X, Ctrl+A) work correctly.
  These are provided natively by ImGui — no additional code is needed.
- The application compiles and runs with the text input visible in the Editor
  panel.

**Suggested tests:**
- `EditorPanel.HasBuffer` — `editor_panel` exposes a non-empty buffer
  accessor.
- `EditorPanel.RenderNoCrash` — call `render()` inside an ImGui frame,
  verify no crash (requires xvfb).
- (Keyboard interaction and clipboard are verified in T3.5 end-to-end.)

---

### T3.4 — Unit Tests for M3

**Parallel group:** C
**Depends on:** T3.3
**Files to create / modify:**
- `tests/test_text_buffer.cpp`
- `tests/CMakeLists.txt` (add new test executable)

**Description:**
Add automated tests for the text buffer and the updated editor panel.
The `text_buffer` tests are pure C++ and do **not** require an ImGui context
or xvfb. The editor panel rendering test requires xvfb (same as M2 panel
tests).

**Acceptance criteria:**
- `test_text_buffer` executable registered with `gtest_discover_tests`.
- At least these text-buffer tests:
  - `TextBuffer.DefaultEmpty` — default-constructed buffer is empty, size 0,
    not modified.
  - `TextBuffer.SetContent` — set content, verify access and size.
  - `TextBuffer.Clear` — clear after set, verify empty and modified.
  - `TextBuffer.ModifiedFlag` — verify flag lifecycle (false → set_content →
    true → set_modified(false) → false).
  - `TextBuffer.MutableAccess` — modify content via `content()` reference,
    verify change is reflected.
- Editor panel rendering test (in existing `test_panels` or new file):
  - Verify `editor_panel::render()` still works inside an ImGui frame after
    the InputTextMultiline change (no crash).
- All existing M0, M1, and M2 tests still pass.

**Suggested tests:**
- (This task *is* the test task.)

---

### T3.5 — End-to-End Verification

**Parallel group:** C
**Depends on:** T3.4 (all prior tasks)
**Files to create / modify:**
- (none — verification only)

**Description:**
Verify the full M3 deliverable works end-to-end.

**Acceptance criteria:**
- `cmake -B build && cmake --build build` succeeds with zero warnings.
- `ctest --test-dir build --output-on-failure` — all tests pass
  (M0 + M1 + M2 + M3).
- Running the binary shows the Editor panel with a multi-line text input
  widget filling the panel area (no placeholder text).
- User can click into the editor and type text.
- Arrow keys move the cursor; Home/End jump to line start/end.
- Shift+arrow keys select text.
- Ctrl+C copies selected text; Ctrl+V pastes from the clipboard.
- Ctrl+A selects all text.
- Tab inserts a tab character (does not change focus).
- The other three panels (Assembly View, Console, Debugger) are unaffected.
- CI workflow is green.

**Suggested tests:**
- (This task *is* the integration test.)
