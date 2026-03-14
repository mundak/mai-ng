# mai-ng

[![CI](https://github.com/mundak/mai-ng/actions/workflows/ci.yml/badge.svg)](https://github.com/mundak/mai-ng/actions/workflows/ci.yml)

Integrated editor, compiler and debugger for the **mai** programming language.

## Prerequisites

* C++20 compiler (GCC 11+ / Clang 14+)
* CMake 3.20+

## Getting Started

```bash
cmake -B build
cmake --build build
ctest --test-dir build
./build/src/mai-ide
```

## Documentation

* [Technical Design Overview](docs/tech-design.md)
* [Milestones](docs/milestones.md)
* [Contributing Guide](docs/contributing.md)
* [Task Overview & Conventions](docs/tasks/overview.md)

## Task Lists (Agent-Ready)

Granular, parallelisable task breakdowns for the first three milestones:

* [M0 — Project Skeleton & CI](docs/tasks/m0-tasks.md) (7 tasks, up to 5 parallel)
* [M1 — Empty Window on Screen](docs/tasks/m1-tasks.md) (9 tasks, up to 3 parallel)
* [M2 — Dockable Panel Layout](docs/tasks/m2-tasks.md) (8 tasks, up to 4 parallel)