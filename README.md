# TIVIDY – Workflow Management System (COS 214)

Generic C++11 workflow engine

## Build, run tests (Linux / macOS / WSL)

```bash
cmake -S . -B build            # add -DTIVIDY_SANITIZE=ON to check for leaks/UB
cmake --build build
ctest --test-dir build --output-on-failure     # or: ./build/tividy_tests
```

Requirements: a C++11 compiler (g++ >= 5 / clang >= 3.5) and CMake >= 3.10.
Test framework: [doctest 2.4.11](https://github.com/doctest/doctest) (MIT licence, vendored in `third_party/`; chosen because it supports C++11).

## Layout

| Path | Contents |
|---|---|
| `tests/` | Automated unit tests |
| `docs/` | Module documentation, design decisions |
| `third_party/` | doctest |

Will generate API docs with `doxygen Doxyfile` (output in `docs/doxygen/`)