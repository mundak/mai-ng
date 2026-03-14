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

## Post-Edit Formatting

After every source file modification (`.cpp`, `.h`, `.inl`), run
`clang-format -i <file>` on the entire modified file to ensure it conforms to
the project's `.clang-format` configuration.

---

## Formatting Overview

The style is based on **WebKit** with significant customizations toward an
**Allman/BSD** brace style.

* **Indent width:** 2 spaces (no tabs)
* **Continuation indent:** 2 spaces
* **Column limit:** 120 characters
* **Access modifier offset:** -2 (aligned with the class keyword)

---

## Braces (Allman Style)

Braces go on their **own line** for all constructs: classes, structs, unions,
enums, functions, control statements (`if`, `for`, `while`, `switch`),
namespaces, lambdas, `catch`, and `else`.

**Always use braces**, even for single-line bodies. Never omit braces for `if`,
`else`, `for`, `while`, or `do` statements.

```cpp
// Good
if (condition)
{
  do_something();
}
else
{
  do_other();
}

// Bad — never omit braces
if (condition)
  do_something();
```

---

## Functions

* **All function definitions** must live in `.cpp` files. Headers contain only
  declarations.
* Return type stays on the same line as the function name.

```cpp
// header (.h) — declaration only
class app
{
public:
  int32_t count() const;
  void init();
};

// source (.cpp) — all definitions
int32_t app::count() const
{
  return m_count;
}
```

### Function Arguments & Parameters

Arguments and parameters are **not bin-packed** — each goes on its own line
when they don't fit on one line (`AlignAfterOpenBracket: AlwaysBreak`).

```cpp
void some_function(
  int32_t first_param,
  int32_t second_param,
  const std::string& third_param)
{
  another_call(
    value_a,
    value_b,
    value_c);
}
```

### Constructor Initializer Lists

Break **before** the comma. Each initializer on its own line.

```cpp
my_class::my_class()
  : m_member_a(0)
  , m_member_b(nullptr)
  , m_member_c("default")
{
}
```

---

## Templates

* Template declarations always break before the templated entity.
* No space after the `template` keyword before `<`.
* **Template function definitions** must live in a separate `.inl` file,
  included at the end of the corresponding `.h` file.

```cpp
// container.h
#pragma once

template<typename T>
class container
{
public:
  void add(const T& value);
  T get(int32_t index) const;
};

#include "container.inl"
```

```cpp
// container.inl
template<typename T>
void container<T>::add(const T& value)
{
  // ...
}

template<typename T>
T container<T>::get(int32_t index) const
{
  // ...
}
```

---

## Naming Conventions

Follow the **C++ Standard Library naming convention** (`snake_case` everywhere).

| Entity | Style | Example |
|--------|-------|---------|
| Files & directories | `snake_case` | `main_loop.cpp`, `editor_panel.h` |
| Namespaces | `snake_case` | `mai`, `math` |
| Classes / structs | `snake_case` | `app`, `vulkan_context` |
| Functions / methods | `snake_case` | `poll_events()`, `render_frame()` |
| Local variables | `snake_case` | `window_width`, `graphics_family` |
| Member variables | `m_` prefix | `m_window`, `m_vk_ctx` |
| Constants / macros | `UPPER_SNAKE_CASE` | `VK_NULL_HANDLE`, `MAI_DEBUG` |

---

## Type Usage

* **Prefer explicit, fixed-width types** over implicit types: `int32_t` over
  `int`, `uint32_t` over `unsigned int`, `int64_t` over `long long`, etc.
* Use `<cstdint>` for fixed-width integer types.
* Use `float`, `double`, `bool`, `char`, and `size_t` as-is where semantically
  appropriate.

---

## `auto` Usage

* **Do not use `auto`** for simple or obvious types. Write the type explicitly.
* **Use `auto` only when** the type is a complex iterator or a heavily templated
  expression where spelling it out reduces readability.
* When in doubt, spell out the type.

```cpp
// Good
int32_t count = 0;
std::string name = get_name();

// Good — complex iterator
auto it = my_map.find(key);

// Bad — do not use auto for simple types
auto count = 0;
auto name = get_name();
```

---

## Pointers & References

Pointer and reference markers bind to the **left** (to the type).

```cpp
int32_t* ptr = nullptr;
const std::string& name = get_name();
```

---

## Static Functions

**Do not use `static` for file-local free functions.** Use an anonymous
`namespace { }` instead.

```cpp
// Good — anonymous namespace
namespace {

  void helper() { }

}

// Bad — static free function
static void helper() { }
```

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

Headers use `.h`, source files use `.cpp`, template definitions use `.inl`.

---

## Header Guards

Use `#pragma once` (supported by all target compilers).

---

## Namespaces

* All namespace content is **indented**.
* **Do not** use closing comments on namespace braces (no `// namespace foo`).
* Namespaces should only be used for **grouping related functions from the same
  area** (e.g., utility modules, subsystem boundaries). Do not wrap entire
  applications or single classes in namespaces unnecessarily.

```cpp
namespace math {

  float lerp(float a, float b, float t);
  float clamp(float value, float min, float max);

}
```

---

## Includes

* Includes are **sorted** alphabetically.
* Include blocks are **regrouped** automatically (system vs. project headers
  separated).

Order:

1. Corresponding header (for `.cpp` files).
2. Project headers (alphabetical).
3. Third-party headers (`imgui.h`, `GLFW/glfw3.h`, etc.).
4. Standard library headers.

Separate each group with a blank line.

---

## Spacing Rules

* **Space after C-style cast:** `(int32_t) value`
* **Space before control-statement parentheses:** `if (`, `for (`, `while (`
* **No space before function call parentheses:** `foo()`, `bar(x)`
* **Space before C++11 braced-init lists:** `vec {1, 2, 3}`
* **No extra spaces inside container literals:** `{1, 2, 3}` not `{ 1, 2, 3 }`

---

## Blank Lines

* Maximum **1** consecutive empty line.
* **Separate definition blocks** with a blank line (between function
  definitions, class definitions, etc.).

---

## Preprocessor Directives

Preprocessor directives are **not indented**.

```cpp
#pragma once

#include <vector>

#ifdef MAI_DEBUG
#define MAI_TRACE(x) spdlog::trace(x)
#endif
```

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
7. **Update `docs/tasks/progress.md`** — mark the task as `done`, add the PR
   number and the completion date. Commit this change with the task deliverables.
8. Commit with a descriptive message referencing the task ID.
9. Open a PR targeting the milestone integration branch.

---

## Logging

**Do not use standard I/O** (`std::cout`, `std::cerr`, `std::cin`, `printf`,
`fprintf`, `puts`, `std::print`, or any other `<iostream>` / `<cstdio>`
facility) for diagnostic output, status messages, or error reporting. All
logging must go through **spdlog**.

spdlog serves three purposes in this project:

1. **Console output** — a console sink prints log messages to the terminal
   during development and CI runs.
2. **File logging** — a rotating-file sink writes log messages to text files
   for offline analysis and bug reports.
3. **IDE log panel** — a custom ImGui sink feeds log messages into the
   Console / Output panel inside the IDE, so users and developers see live
   diagnostics in the application itself.

### Usage

```cpp
#include <spdlog/spdlog.h>

spdlog::info("Mai IDE v{}.{}", major, minor);
spdlog::warn("frame took {} ms", elapsed_ms);
spdlog::error("failed to open file: {}", path);
```

### Log Levels

Use log levels consistently:

| Level | When to use |
|-------|-------------|
| `trace` | Very detailed diagnostic information (disabled in Release) |
| `debug` | Diagnostic information useful during development |
| `info` | Normal operational messages (startup, shutdown, milestones) |
| `warn` | Recoverable problems that deserve attention |
| `error` | Failures that prevent an operation from completing |
| `critical` | Fatal errors that will cause the application to exit |

### Sink Configuration

Sink setup (console, file, ImGui panel) is handled centrally during
application initialisation. Individual modules should only call the `spdlog`
logging macros/functions — they must **never** create sinks or configure the
global logger.

---

## Dependencies

* Managed via CMake `FetchContent`.
* New dependencies must be justified and documented.
* Prefer header-only or small, well-maintained libraries.
* Current dependencies:
  - **GLFW 3.4** — windowing / input
  - **Dear ImGui (docking branch)** — GUI framework
  - **spdlog 1.15** — logging (console, file, and custom ImGui sinks)
  - **GoogleTest 1.14** — testing
