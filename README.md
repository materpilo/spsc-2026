# spsc

A C++20 single-producer, single-consumer queue project. The repository contains
the header-only `spsc` library and the `spsc_tests` GoogleTest executable. CMake
downloads GoogleTest via CPM when tests are enabled.

## Requirements

- CMake 3.21 or newer
- GCC
- Ninja
- Internet access on the first configure to download CPM and GoogleTest

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

## References

Project structure is inspired by https://github.com/cpp-best-practices/cmake_template, especially compiler warning flags and linter/formatter configuration.
