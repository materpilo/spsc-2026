# SPSC

A C++20 single-producer, single-consumer queue project. The repository contains
the header-only `spsc` library and the `spsc_tests` GoogleTest executable. CMake
downloads GoogleTest via CPM when tests are enabled.

## Requirements

- CMake 3.21 or newer
- GCC
- Ninja
- Internet access on the first configure to download CPM and GoogleTest
- (optional) clang-format and clang-tidy executables for code quality analysis targets

**Tested on following versions**

| Executable | Version used |
| --- | --- |
| CMake | 3.28.3 |
| GCC / G++ | 13.3.0 |
| Ninja | 1.11.1 |
| clang-format | 18.1.3 |
| clang-tidy | 18.1.3 |

## Configure, build, and test

From the repository root:

```sh
cmake --preset gcc-ninja
cmake --build --preset gcc-ninja
ctest --preset gcc-ninja
```

The preset uses Ninja and GCC and compiles the test executable with Debug flags
and AddressSanitizer/UndefinedBehaviorSanitizer. As a header-only library,
`spsc` is compiled as part of its consumers.

## Optional code checks

If `clang-tidy` and/or `clang-format` are installed, CMake provides targets for
running them on demand. They are not run as part of a normal build. Configure
the project first, then run:

```sh
cmake --build --preset gcc-ninja --target spsc_tidy_check
cmake --build --preset gcc-ninja --target spsc_format_check
```

To format the C++ sources in place with the repository's clang-format settings:

```sh
cmake --build --preset gcc-ninja --target spsc_format
```

If a checker is not installed, CMake emits a warning and continues; the
corresponding target is unavailable.

## Continuous integration

The GitHub Actions CI job runs on pushes to `main`. It provisions a fresh
Ubuntu 24.04 runner, installs the build and analysis tools, then configures the
project and runs formatting checks, clang-tidy, the build, and tests. This is a
simple setup: a new runner image is re-created on every run. 

Possible improvements: 
- Store the image permanently and reuse it.
- Trigger the build only when relevant files were modified

## References

Project structure is inspired by https://github.com/cpp-best-practices/cmake_template, especially compiler warning flags and linter/formatter configuration.
