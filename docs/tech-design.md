# Mai IDE — Technical Design Overview

## 1. Vision

Mai IDE is an integrated editor, compiler and debugger for the **mai** programming language (a custom language based on C). The application provides a live-compilation workflow: as the user types source code, it is compiled in the background into machine instructions and the resulting assembly is displayed in a panel next to the editor. When the user presses **Run**, the cached binary is written to disk, executed inside QEMU, and a debugger is attached—showing the current instruction and corresponding source line in real time.

The entire UI is built with **Dear ImGui**, targeting a lightweight, single-window desktop application.

---

## 2. High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                   Application Shell                     │
│              (ImGui / platform backend)                  │
├──────────┬──────────────┬──────────────┬────────────────┤
│  Editor  │ Assembly View│   Debugger   │  Console /     │
│  Module  │              │   Module     │  Output Panel  │
├──────────┴──────┬───────┴──────────────┴────────────────┤
│                 │                                        │
│   Compiler Pipeline          Execution Engine            │
│   (Lexer → Parser → AST     (Binary Cache → Disk →      │
│    → Code Generator)          QEMU → GDB Stub)          │
└─────────────────────────────────────────────────────────┘
```

Each box above represents a major module described in the sections that follow.

---

## 3. Module Descriptions

### 3.1 Application Shell

**Responsibility:** bootstrap the application, create the OS window, initialize the Dear ImGui context and rendering backend, and drive the main loop.

| Aspect | Detail |
|--------|--------|
| Windowing | GLFW or SDL2 (platform abstraction) |
| Rendering | OpenGL 3 / Vulkan backend for ImGui |
| Layout | Dockable panels managed by ImGui's docking branch |
| Event loop | Single-threaded UI loop; heavy work is offloaded to background threads |

### 3.2 Editor Module

**Responsibility:** provide a text-editing surface for mai source files with syntax highlighting and basic editing features.

* Renders a multi-line text buffer inside an ImGui panel.
* Performs syntax highlighting using token information produced by the compiler pipeline's lexer.
* Tracks cursor position and selection; maps edits to the internal source buffer that the compiler pipeline consumes.
* Notifies the compiler pipeline on every meaningful edit (debounced) so that background recompilation can start.

### 3.3 Compiler Pipeline

**Responsibility:** translate mai source code into target machine code (assembly / binary).

The pipeline follows classic compiler stages:

1. **Lexer** — tokenises the source text into a stream of tokens (keywords, identifiers, literals, operators, etc.).
2. **Parser** — consumes tokens and builds an Abstract Syntax Tree (AST). Produces diagnostics (errors/warnings) surfaced in the editor.
3. **Semantic Analysis** — type checking, scope resolution, and other static checks performed on the AST.
4. **Code Generator** — walks the validated AST and emits target assembly (initially targeting x86-64 or RISC-V). Produces:
   * An assembly text listing (consumed by the Assembly View).
   * A relocatable object / flat binary (consumed by the Execution Engine).
5. **Source Map** — maintains a bidirectional mapping between source lines/columns and generated assembly addresses. Used by both the Assembly View and the Debugger.

The pipeline is designed to run **incrementally** on a background thread, re-compiling only the changed portions of the source when possible, and caching results.

### 3.4 Assembly View

**Responsibility:** display the generated assembly alongside the source code.

* Reads the assembly listing and source map produced by the compiler pipeline.
* Renders assembly instructions in a scrollable ImGui panel adjacent to the editor.
* Highlights the assembly range that corresponds to the currently selected source line (and vice-versa), using the source map.
* Updates automatically whenever the compiler pipeline produces a new build.

### 3.5 Build Cache

**Responsibility:** store and manage compiled artefacts so that the run/debug cycle is as fast as possible.

* Keeps the most recent successful binary in memory.
* On **Run**, writes the binary to a temporary file on disk for QEMU to load.
* Invalidates the cache whenever the compiler pipeline produces a newer binary.
* Manages temporary files and cleans up on exit.

### 3.6 Execution Engine

**Responsibility:** run the compiled binary inside QEMU and manage the process lifecycle.

* Spawns a QEMU system or user-mode emulation process, loading the cached binary.
* Starts QEMU with the GDB stub enabled (`-s -S` flags) so the Debugger Module can attach.
* Captures stdout/stderr from the guest and forwards them to the Console / Output Panel.
* Handles start, pause, resume, stop and restart actions triggered from the UI.

### 3.7 Debugger Module

**Responsibility:** provide interactive debugging by communicating with QEMU's GDB remote stub.

* Implements (or wraps) the GDB Remote Serial Protocol to connect to QEMU's stub.
* Supports core debug operations: set/remove breakpoints, step, continue, inspect registers and memory.
* Uses the source map from the compiler pipeline to translate instruction addresses back to source lines, highlighting the current execution point in both the Editor and the Assembly View.
* Exposes a watch/locals panel showing variable values derived from debug information.

### 3.8 Console / Output Panel

**Responsibility:** display program output and build diagnostics.

* Shows compiler errors and warnings produced by the compiler pipeline.
* Streams stdout/stderr from the running program (via the Execution Engine).
* Optionally accepts stdin input to forward to the guest process.

---

## 4. Data Flow Summary

```
 User edits source
        │
        ▼
  ┌───────────┐   tokens   ┌────────┐   AST    ┌───────────┐
  │   Lexer   │ ─────────► │ Parser │ ───────► │ Sem. Anal.│
  └───────────┘            └────────┘          └─────┬─────┘
                                                     │
                                          validated AST
                                                     │
                                                     ▼
                                              ┌────────────┐
                                              │  Code Gen  │
                                              └──────┬─────┘
                                                     │
                                   ┌─────────────────┼─────────────────┐
                                   ▼                 ▼                 ▼
                            Assembly Text       Binary Blob       Source Map
                                   │                 │                 │
                                   ▼                 ▼                 ▼
                           Assembly View       Build Cache        Debugger &
                                                    │            Assembly View
                                                    ▼
                                              Execution Engine
                                              (QEMU + GDB stub)
                                                    │
                                                    ▼
                                            Console / Output
```

---

## 5. Technology Choices (initial)

| Area | Choice | Rationale |
|------|--------|-----------|
| Language | C++ (C++17 or later) | Performance, direct ImGui integration, low-level control for compiler work |
| UI framework | Dear ImGui (docking branch) | Immediate-mode GUI, lightweight, easy panel layout |
| Windowing | GLFW | Minimal, well-supported, pairs well with ImGui |
| Rendering backend | OpenGL 3.3+ | Wide compatibility; can migrate to Vulkan later |
| Emulation | QEMU (user-mode) | Mature, supports GDB stub out of the box |
| Debugger protocol | GDB Remote Serial Protocol | Standard, well-documented, directly supported by QEMU |
| Build system | CMake | Industry standard for C++ projects |

---

## 6. Open Questions

* **Target architecture** — Should the first target be x86-64 (simplest for host-native debugging) or RISC-V (cleaner ISA, always requires QEMU)?
* **Incremental compilation granularity** — Function-level vs. file-level re-compilation on edits.
* **Language specification** — The exact deviations of "mai" from C need to be documented separately.
* **Plugin / extension model** — Decide early whether the architecture should support third-party extensions.
