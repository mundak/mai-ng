# M0 — Project Skeleton & CI — Task List

**Milestone goal:** A C++20 project that compiles, links and runs; a CI
pipeline that builds and tests on every push.

**Demo:** `cmake -B build && cmake --build build && ./build/src/mai-ide` logs
`"Mai IDE"` to the console. CI badge is green.

**Prerequisites:** none (this is the first milestone).

---

## Parallel Groups

```
Group A (5 tasks, all independent — run in parallel)
  T0.1  Root CMakeLists.txt
  T0.2  src/main.cpp
  T0.3  .gitignore
  T0.4  Test infrastructure (GoogleTest + smoke test)
  T0.5  GitHub Actions CI workflow

Group B (1 task, depends on all of Group A)
  T0.6  Update README.md with build instructions & CI badge

Group C (1 task, depends on Group B)
  T0.7  End-to-end verification
```

---

## Tasks

### T0.1 — Root CMakeLists.txt

**Parallel group:** A
**Depends on:** none
**Files to create / modify:**
- `CMakeLists.txt` (project root)
- `src/CMakeLists.txt`

**Description:**
Create the root CMake project file and a `src/` subdirectory CMake file that
builds the `mai-ide` executable.

**Acceptance criteria:**
- `cmake_minimum_required` is 3.20 or later.
- Project name is `mai-ide`, version `0.1.0`, language `CXX`.
- C++20 is required (`CMAKE_CXX_STANDARD 20`, `CMAKE_CXX_STANDARD_REQUIRED ON`,
  `CMAKE_CXX_EXTENSIONS OFF`).
- Compiler warnings enabled: `-Wall -Wextra -Wpedantic` (GCC/Clang) or `/W4`
  (MSVC).
- `CMAKE_EXPORT_COMPILE_COMMANDS ON` for tooling.
- spdlog is declared via `FetchContent_Declare` (repo:
  `https://github.com/gabime/spdlog.git`, tag: `v1.15.0`).
- `src/CMakeLists.txt` defines an executable target `mai-ide` from `main.cpp`
  and links it to `spdlog::spdlog`.
- An option `MAI_BUILD_TESTS` (default `ON`) gates `add_subdirectory(tests)`.
- `enable_testing()` is called when tests are enabled.

**Suggested tests:**
- (No unit test — this is build infrastructure. Verified by T0.7.)

---

### T0.2 — src/main.cpp

**Parallel group:** A
**Depends on:** none
**Files to create / modify:**
- `src/main.cpp`

**Description:**
Create the minimal entry point for the Mai IDE application.

**Acceptance criteria:**
- Logs exactly `Mai IDE` to the console using `spdlog::info`.
- Returns `EXIT_SUCCESS` (0).
- Uses `<spdlog/spdlog.h>` and `<cstdlib>`.
- Does **not** use `<iostream>`, `std::cout`, `printf`, or any other standard
  I/O for output (see contributing.md — Logging).
- No other logic.

**Suggested tests:**
- (Tested indirectly by the smoke test in T0.4 and by T0.7.)

---

### T0.3 — .gitignore

**Parallel group:** A
**Depends on:** none
**Files to create / modify:**
- `.gitignore`

**Description:**
Create a `.gitignore` tailored to a C++/CMake project.

**Acceptance criteria:**
- Ignores: `build/`, `cmake-build-*/`, `out/`, `CMakeCache.txt`, `CMakeFiles/`,
  `cmake_install.cmake`, `Makefile`, `CTestTestfile.cmake`, `Testing/`, `_deps/`,
  `compile_commands.json`.
- Ignores IDE files: `.vscode/`, `.idea/`, `*.swp`, `*.swo`, `*~`, `.cache/`.
- Ignores compiled objects: `*.o`, `*.obj`, `*.a`, `*.lib`, `*.so`, `*.dylib`,
  `*.dll`, `*.exe`.
- Ignores OS files: `.DS_Store`, `Thumbs.db`.

**Suggested tests:**
- (No test — verified by inspection.)

---

### T0.4 — Test Infrastructure (GoogleTest + Smoke Test)

**Parallel group:** A
**Depends on:** none
**Files to create / modify:**
- `tests/CMakeLists.txt`
- `tests/test_main.cpp`

**Description:**
Set up GoogleTest via CMake `FetchContent` and create a minimal smoke test that
validates the test infrastructure works.

**Acceptance criteria:**
- `tests/CMakeLists.txt` fetches GoogleTest v1.14.0 (or later stable) using
  `FetchContent_Declare` / `FetchContent_MakeAvailable`.
- `gtest_force_shared_crt` is set `ON` for Windows compatibility.
- `include(GoogleTest)` is called.
- A test executable `test_main` is defined and registered via
  `gtest_discover_tests(test_main)`.
- `tests/test_main.cpp` contains at least two tests:
  - `SmokeTest.TrueIsTrue` — `EXPECT_TRUE(true)`.
  - `SmokeTest.ProjectName` — verifies the string `"Mai IDE"` compiles and
    compares equal.
- `ctest --test-dir build` runs the tests and reports pass.

**Suggested tests:**
- The file itself *is* the test. Verified by running `ctest`.

---

### T0.5 — GitHub Actions CI Workflow

**Parallel group:** A
**Depends on:** none
**Files to create / modify:**
- `.github/workflows/ci.yml`

**Description:**
Create a GitHub Actions workflow that builds and tests the project on every
push and pull request.

**Acceptance criteria:**
- Workflow name: `CI`.
- Triggers: `push` (all branches), `pull_request` (all branches).
- Runs on `ubuntu-latest`.
- Steps:
  1. `actions/checkout@v4`
  2. Install build tools: `sudo apt-get update && sudo apt-get install -y cmake g++`
     (or use a CMake action).
  3. Configure: `cmake -B build -DCMAKE_BUILD_TYPE=Release`
  4. Build: `cmake --build build --parallel`
  5. Test: `ctest --test-dir build --output-on-failure`
  6. Run binary: `./build/src/mai-ide` (verify it exits 0).
- The workflow YAML is valid and lints cleanly.

**Suggested tests:**
- (No unit test — verified by pushing to GitHub and seeing a green badge.)

---

### T0.6 — Update README.md

**Parallel group:** B
**Depends on:** T0.1, T0.2, T0.3, T0.4, T0.5
**Files to create / modify:**
- `README.md`

**Description:**
Update the project README with build instructions, a CI status badge, and
links to the new task lists.

**Acceptance criteria:**
- CI badge at the top (GitHub Actions badge for the `CI` workflow).
- "Getting Started" section with:
  ```
  cmake -B build
  cmake --build build
  ctest --test-dir build
  ./build/src/mai-ide
  ```
- "Documentation" section links to: tech-design.md, milestones.md,
  contributing.md, tasks/overview.md.
- "Prerequisites" section listing: C++20 compiler (GCC 11+ / Clang 14+),
  CMake 3.20+.

**Suggested tests:**
- (No test — verified by inspection.)

---

### T0.7 — End-to-End Verification

**Parallel group:** C
**Depends on:** T0.6 (all prior tasks)
**Files to create / modify:**
- (none — this is a verification-only task)

**Description:**
Verify the full M0 deliverable works end-to-end.

**Acceptance criteria:**
- Fresh clone of the branch → `cmake -B build && cmake --build build` succeeds
  with zero errors and zero warnings.
- `./build/src/mai-ide` logs `Mai IDE` to the console and exits with code 0.
- `ctest --test-dir build` reports all tests passed.
- CI workflow runs green on the PR.
- `.gitignore` correctly excludes build artefacts.

**Suggested tests:**
- (This task *is* the integration test.)
