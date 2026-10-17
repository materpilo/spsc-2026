cmake_minimum_required(VERSION 3.21)

project(spsc LANGUAGES CXX)

include(CTest)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This project is configured to use GCC.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SPSC_WARNING_OPTIONS
    -Werror
    -Wall
    -Wextra
)

# Files with format specification for tidy and format checks
# They are used when adding files in each component
set(SPSC_CLANG_TIDY_CONFIG "${PROJECT_SOURCE_DIR}/clang-tidy")
set(SPSC_CLANG_FORMAT_CONFIG "${PROJECT_SOURCE_DIR}/clang-format")

include(cmake/ClangTidy.cmake)
include(cmake/ClangFormat.cmake)

add_subdirectory(src)

if(BUILD_TESTING)
    include(cmake/CPM.cmake)
    add_subdirectory(test)
endif()
