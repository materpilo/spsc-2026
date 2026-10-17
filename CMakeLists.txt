cmake_minimum_required(VERSION 3.21)

project(spsc LANGUAGES CXX)

include(CTest)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This project is configured to use GCC.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SPSC_WARNING_OPTIONS
    -Werror
    -Wall
    -Wextra
)

add_subdirectory(src)

if(BUILD_TESTING)
    set(CPM_DOWNLOAD_VERSION 0.40.2)
    set(CPM_DOWNLOAD_LOCATION
        "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake"
    )

    if(NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
        file(DOWNLOAD
            "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
            "${CPM_DOWNLOAD_LOCATION}"
            TLS_VERIFY ON
            STATUS CPM_DOWNLOAD_STATUS
        )
        list(GET CPM_DOWNLOAD_STATUS 0 CPM_DOWNLOAD_RESULT)
        if(NOT CPM_DOWNLOAD_RESULT EQUAL 0)
            list(GET CPM_DOWNLOAD_STATUS 1 CPM_DOWNLOAD_ERROR)
            message(FATAL_ERROR "Failed to download CPM.cmake: ${CPM_DOWNLOAD_ERROR}")
        endif()
    endif()
    include("${CPM_DOWNLOAD_LOCATION}")

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    CPMAddPackage("gh:google/googletest@1.15.2")

    add_subdirectory(test)
endif()
