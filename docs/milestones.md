# Mai IDE — Milestones

Each milestone produces a **runnable product** that can be launched and demonstrated.
Milestones are intentionally small to allow rapid iteration and course-correction
between any two milestones.

> **Traceability:** section numbers in parentheses (e.g. *TDD §3.1*) refer to the
> [Technical Design Overview](tech-design.md).

---

## M0 — Project Skeleton & CI

**Goal:** a C++ project that compiles, links and runs; a CI pipeline that
builds on every push.

| Deliverable | Detail |
|-------------|--------|
| `CMakeLists.txt` | Minimal CMake project (C++17) |
| `src/main.cpp` | Prints `"Mai IDE"` and exits |
| GitHub Actions workflow | Builds the project on Ubuntu; reports pass/fail |

**Demo:** clone → `cmake -B build && cmake --build build` → run the binary →
see `"Mai IDE"` printed. CI badge is green.

---

## M1 — Empty Window on Screen

*TDD §3.1 — Application Shell*

**Goal:** open an OS window with a Dear ImGui context; nothing else drawn yet.

| Deliverable | Detail |
|-------------|--------|
| GLFW + OpenGL 3.3 initialisation | Window creation, GL context |
| Dear ImGui (docking branch) bootstrap | ImGui context, font atlas, render loop |
| Graceful shutdown | ESC or window-close exits cleanly |

**Demo:** launch the binary → a blank window titled *"Mai IDE"* appears.

---

## M2 — Dockable Panel Layout

*TDD §3.1 — Application Shell (layout)*

**Goal:** four named, dockable panels arranged in the default layout described
in the TDD.

| Deliverable | Detail |
|-------------|--------|
| Editor panel | Empty panel, left side |
| Assembly View panel | Empty panel, right of editor |
| Console / Output panel | Empty panel, bottom |
| Debugger panel | Empty panel, right or tabbed |
| Default dock layout | Panels arranged automatically on first launch |

**Demo:** launch → four labelled panels visible; drag a panel tab to rearrange.

---

## M3 — Text Input in Editor Panel

*TDD §3.2 — Editor Module (text buffer, cursor, selection)*

**Goal:** the user can type, select, copy and paste text inside the Editor
panel.

| Deliverable | Detail |
|-------------|--------|
| Multi-line text widget | `ImGui::InputTextMultiline` or equivalent |
| Cursor movement & selection | Arrow keys, Shift-select, Home/End |
| Copy / Paste | Ctrl-C, Ctrl-V via OS clipboard |

**Demo:** launch → click the Editor panel → type code → select text → paste
it elsewhere in the buffer.

---

## M4 — Minimal Lexer

*TDD §3.3 — Compiler Pipeline (Lexer)*

**Goal:** tokenise a tiny subset of the mai language; validate with unit tests.

| Deliverable | Detail |
|-------------|--------|
| Token types | Keywords (`int`, `return`), identifiers, integer literals, operators (`+`, `-`, `*`, `/`, `=`), punctuation (`{`, `}`, `(`, `)`, `;`) |
| Lexer function | `std::vector<Token> lex(std::string_view source)` |
| Unit tests | Cover valid token streams and edge cases (empty input, unknown chars) |
| CTest integration | `ctest` runs the lexer tests in CI |

**Demo:** CI runs → lexer tests pass (green).

---

## M5 — Syntax Highlighting

*TDD §3.2 — Editor Module (syntax highlighting)*

**Goal:** feed lexer tokens into the editor to colour keywords, literals, etc.

| Deliverable | Detail |
|-------------|--------|
| Token-based colouring | Keywords in one colour, literals in another, identifiers default |
| Live re-lex on edit | Re-tokenise the buffer after each change (debounced) |

**Demo:** launch → type `int main() { return 42; }` → keywords and numbers
are coloured.

---

## M6 — Minimal Parser & AST

*TDD §3.3 — Compiler Pipeline (Parser)*

**Goal:** parse the smallest possible valid program into an AST.

| Deliverable | Detail |
|-------------|--------|
| AST node types | `Program`, `FunctionDecl`, `ReturnStmt`, `IntegerLiteral` |
| Parser function | `ParseResult parse(std::vector<Token> const&)` |
| Diagnostics list | Collect errors with line/column info |
| Unit tests | Valid and invalid programs |

**Demo:** CI runs → parser tests pass (green).

---

## M7 — Error Display in Console Panel

*TDD §3.8 — Console / Output Panel (compiler errors)*

**Goal:** surface parser diagnostics in the Console panel so the user sees
errors as they type.

| Deliverable | Detail |
|-------------|--------|
| Compiler error list | Console panel renders errors from the parser |
| Live update | Re-parse on edit (debounced); refresh error list |
| Error format | `line:col: error: message` |

**Demo:** launch → type invalid code in the editor → see errors appear in
the Console panel; fix the code → errors disappear.

---

## M8 — Semantic Analysis (Minimal)

*TDD §3.3 — Compiler Pipeline (Semantic Analysis)*

**Goal:** perform basic type checking and scope resolution on the AST.

| Deliverable | Detail |
|-------------|--------|
| Type checker | Validate return type matches function signature |
| Scope resolution | Report undeclared identifiers |
| Diagnostics | Semantic errors surfaced in the Console panel alongside parse errors |
| Unit tests | Valid and invalid semantic cases |

**Demo:** type `int main() { return "hello"; }` → see a type-mismatch error
in the Console panel.

---

## M9 — Code Generator (Trivial)

*TDD §3.3 — Compiler Pipeline (Code Generator)*

**Goal:** emit assembly for the simplest possible program
(`int main() { return <N>; }`).

| Deliverable | Detail |
|-------------|--------|
| Target ISA | x86-64 **or** RISC-V (decide in this milestone; see TDD §6) |
| Codegen function | `CodegenResult codegen(AST const&)` producing assembly text + binary blob |
| Unit tests | Verify expected instruction sequences |

**Demo:** CI runs → codegen tests pass (green).

---

## M10 — Assembly View

*TDD §3.4 — Assembly View*

**Goal:** display the generated assembly in the Assembly View panel, updating
live as the user edits.

| Deliverable | Detail |
|-------------|--------|
| Assembly rendering | Scrollable list of instructions in the panel |
| Live update | Re-run compiler pipeline on edit → refresh assembly |
| Empty/error state | Show "no output" or errors when compilation fails |

**Demo:** launch → type `int main() { return 42; }` → see assembly in the
right panel; change `42` to `7` → assembly updates.

---

## M11 — Source Map & Cross-Highlighting

*TDD §3.3 item 5, §3.4 — Source Map*

**Goal:** bidirectional mapping between source lines and assembly addresses;
clicking a source line highlights the matching assembly and vice-versa.

| Deliverable | Detail |
|-------------|--------|
| Source map structure | `SourceMap` mapping line → address range and address → line |
| Editor → Assembly highlight | Click/cursor on a source line → highlight assembly range |
| Assembly → Editor highlight | Click an instruction → highlight the source line |
| Unit tests | Verify round-trip mapping |

**Demo:** launch → type a valid program → click a source line → see the
corresponding assembly instructions highlighted; click an instruction → see
the source line highlighted.

---

## M12 — Build Cache & Binary Emission

*TDD §3.5 — Build Cache*

**Goal:** cache the latest successful binary in memory and write it to a
temporary file when the user triggers "Build".

| Deliverable | Detail |
|-------------|--------|
| In-memory cache | Store binary blob from codegen |
| Cache invalidation | Replace on new successful build |
| "Build" action | Menu item or button; writes binary to a temp file |
| Temp-file cleanup | Delete temp files on exit |

**Demo:** launch → type a valid program → click *Build* → temp file appears
on disk (verify with `ls /tmp`).

---

## M13 — QEMU Execution

*TDD §3.6 — Execution Engine*

**Goal:** spawn QEMU loading the cached binary and capture its exit code.

| Deliverable | Detail |
|-------------|--------|
| QEMU process spawn | Launch `qemu-system-*` or `qemu-user` with the binary |
| Exit code capture | Report success/failure in the Console panel |
| "Run" action | Button that builds (if needed) and then executes |

**Demo:** launch → type `int main() { return 42; }` → click *Run* → Console
shows "Process exited with code 42".

---

## M14 — Console Output Streaming

*TDD §3.8 — Console / Output Panel (stdout/stderr)*

**Goal:** stream the running program's stdout/stderr into the Console panel
in real time.

| Deliverable | Detail |
|-------------|--------|
| Pipe capture | Redirect QEMU stdout/stderr to the UI thread |
| Live rendering | Append lines to the Console as they arrive |
| Scroll-to-bottom | Auto-scroll on new output |

**Demo:** extend the language just enough to emit a syscall write (or use an
intrinsic); run the program → see text appear in the Console panel.

---

## M15 — GDB Stub Connection

*TDD §3.7 — Debugger Module*

**Goal:** attach to QEMU's GDB remote stub and send basic commands.

| Deliverable | Detail |
|-------------|--------|
| GDB Remote Serial Protocol client | Connect to `localhost:<port>` |
| Handshake | `qSupported`, read target description |
| Continue / Halt | Send `c` (continue) and `Ctrl-C` (break) |
| "Debug" action | Button that builds, launches QEMU with `-s -S`, and attaches |

**Demo:** launch → click *Debug* → program starts paused → click *Continue*
→ program runs to completion.

---

## M16 — Breakpoints & Stepping

*TDD §3.7 — Debugger Module (breakpoints, step)*

**Goal:** set breakpoints from the editor gutter; step through instructions.

| Deliverable | Detail |
|-------------|--------|
| Set / remove breakpoint | Click in the editor gutter to toggle |
| GDB `Z0`/`z0` packets | Insert/remove software breakpoints |
| Step over / Step in | GDB `s` (single-step) |
| Current-line highlight | Highlight the active source line and assembly instruction during a break |

**Demo:** launch → set a breakpoint on `return 42;` → click *Debug* →
execution pauses at that line → step → execution advances.

---

## M17 — Register & Memory Inspection

*TDD §3.7 — Debugger Module (inspect registers and memory)*

**Goal:** display CPU registers and allow memory inspection while paused.

| Deliverable | Detail |
|-------------|--------|
| Register panel | Show general-purpose registers, updated on each stop |
| Memory view | Hex dump of a user-specified address range |
| GDB `g` / `m` packets | Read registers and memory |

**Demo:** pause at a breakpoint → see register values in the Debugger panel;
type a memory address → see hex dump.

---

## M18 — Language: Expressions & Local Variables

*TDD §3.3, §6 — Language specification*

**Goal:** extend the mai language to support arithmetic expressions, local
variable declarations and assignments.

| Deliverable | Detail |
|-------------|--------|
| Lexer additions | `let` keyword (or C-style declarations), `==`, `!=`, `<`, `>` |
| AST additions | `BinaryExpr`, `VarDecl`, `VarRef`, `Assignment` |
| Codegen | Register allocation or stack-based locals; arithmetic instructions |
| Unit tests | Expression evaluation, variable read/write |

**Demo:** type `int main() { int x = 2 + 3; return x; }` → assembly shows
arithmetic → *Run* → Console shows "Process exited with code 5".

---

## M19 — Watch / Locals Panel

*TDD §3.7 — Debugger Module (watch/locals)*

**Goal:** show local variable names and values in the Debugger panel while
paused.

| Deliverable | Detail |
|-------------|--------|
| Debug info emission | Codegen emits variable-to-stack-offset metadata |
| Locals panel | Lists variable names and current values |
| Watch expressions | User can type an expression to evaluate |

**Demo:** pause at a breakpoint after `int x = 5;` → Debugger panel shows
`x = 5`.

---

## M20 — Language: Control Flow

*TDD §3.3 — Language specification*

**Goal:** add `if`/`else` and `while` statements.

| Deliverable | Detail |
|-------------|--------|
| Lexer/Parser additions | `if`, `else`, `while` keywords; condition expressions |
| AST additions | `IfStmt`, `WhileStmt` |
| Codegen | Conditional jumps, loop back-edges |
| Unit tests | Branch coverage, infinite-loop guard |

**Demo:** type a program with a loop that sums 1..10 → *Run* → Console shows
"Process exited with code 55".

---

## M21 — Language: Functions & Calls

*TDD §3.3 — Language specification*

**Goal:** support multiple function declarations and function calls.

| Deliverable | Detail |
|-------------|--------|
| Lexer/Parser additions | Function declarations with parameters, `call` semantics |
| AST additions | `FunctionDecl` with params, `CallExpr` |
| Codegen | Calling convention (cdecl or custom), stack frames |
| Debugger | Call-stack view in the Debugger panel |
| Unit tests | Recursive and multi-function programs |

**Demo:** write a recursive factorial → *Debug* → step through calls → see
call stack in the Debugger panel.

---

## M22 — Language: Pointers & Arrays

*TDD §3.3, §6 — Language specification*

**Goal:** add pointer types, address-of (`&`), dereference (`*`), and
fixed-size arrays.

| Deliverable | Detail |
|-------------|--------|
| Lexer/Parser additions | `*`, `&` in type/expression position; `[]` indexing |
| AST additions | `PointerType`, `AddressOf`, `Deref`, `ArrayDecl`, `IndexExpr` |
| Codegen | Load/store through pointers; array base + offset |
| Debugger | Memory view auto-navigates to pointer target |
| Unit tests | Pointer arithmetic, array access |

**Demo:** write a program that fills an array and sums it → *Run* → correct
result. Pause in debugger → inspect pointer and array contents.

---

## M23 — Incremental Compilation

*TDD §3.3 — Compiler Pipeline (incremental, background thread)*

**Goal:** re-compile only changed functions rather than the entire file;
run the pipeline on a background thread.

| Deliverable | Detail |
|-------------|--------|
| Function-level caching | Cache codegen output per function |
| Dirty tracking | Mark only changed functions for recompilation |
| Background thread | Pipeline runs off the UI thread; results posted back |
| Unit tests | Verify partial re-compilation correctness |

**Demo:** edit a single function in a multi-function file → observe only that
function's assembly changes; UI remains responsive during compilation.

---

## M24 — File Management

*TDD §3.2 — Editor Module*

**Goal:** open, save, and manage source files from disk.

| Deliverable | Detail |
|-------------|--------|
| Open file dialog | Native or ImGui file dialog |
| Save / Save-As | Write buffer to disk |
| Recent files | Remember and list recently opened files |
| Unsaved-changes guard | Prompt before closing an unsaved file |

**Demo:** *File → Open* → pick a `.mai` file → it loads in the editor; edit →
*File → Save* → changes persisted.

---

## M25 — Stdin Forwarding

*TDD §3.8 — Console / Output Panel (stdin)*

**Goal:** allow the user to type input that is forwarded to the running
program's stdin.

| Deliverable | Detail |
|-------------|--------|
| Input line in Console | Text field at the bottom of the Console panel |
| Pipe to QEMU stdin | Forward user input to the guest process |

**Demo:** write a program that reads a number and prints it doubled → *Run* →
type `21` in the Console → see `42` printed.

---

## M26 — Polish & Packaging

**Goal:** harden the application and prepare a distributable package.

| Deliverable | Detail |
|-------------|--------|
| Error recovery | Graceful handling of QEMU crashes, GDB disconnects |
| Keyboard shortcuts | Consistent shortcuts for Build, Run, Debug, Step, etc. |
| Application icon & title | Branded window |
| Installer / portable zip | CMake CPack or equivalent |
| Documentation | User-facing README with screenshots |

**Demo:** download the release package → install → launch Mai IDE → full
workflow from typing code to debugging.

---

## Milestone Dependency Graph

```
M0 ──► M1 ──► M2 ──► M3
                        │
              ┌─────────┴──────────┐
              ▼                    ▼
             M4 ──► M5           M24
              │
              ▼
             M6 ──► M7 ──► M8
              │
              ▼
             M9 ──► M10 ──► M11 ──► M12 ──► M13 ──► M14
                                              │
                                              ▼
                                    M15 ──► M16 ──► M17
                                                     │
                                                     ▼
                                                    M19
              │
              └──► M18 ──► M20 ──► M21 ──► M22
                                            │
                                            ▼
                                           M23

M25 depends on M14
M26 depends on all previous milestones
```

> Milestones on separate branches of the graph can be worked on in parallel
> (e.g. M24 can be developed alongside the compiler milestones).
