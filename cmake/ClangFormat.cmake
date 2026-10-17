find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-20 clang-format-19 clang-format-18
        clang-format-17 clang-format-16 clang-format-15 clang-format-14
)

if(CLANG_FORMAT_EXECUTABLE)
    set(SPSC_FORMAT_SOURCES
        "${PROJECT_SOURCE_DIR}/src/spsc.cpp"
        "${PROJECT_SOURCE_DIR}/src/spsc.hpp"
    )
    if(BUILD_TESTING)
        list(APPEND SPSC_FORMAT_SOURCES
            "${PROJECT_SOURCE_DIR}/test/spsc_test.cpp"
        )
    endif()

    add_custom_target(spsc_format_check
        COMMAND "${CMAKE_COMMAND}" -E echo
            "clang-format will check: ${SPSC_FORMAT_SOURCES}"
        COMMAND "${CLANG_FORMAT_EXECUTABLE}"
            "--style=file:${PROJECT_SOURCE_DIR}/clang-format"
            --dry-run
            --Werror
            --verbose
            ${SPSC_FORMAT_SOURCES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Checking C++ formatting with clang-format"
        VERBATIM
    )

    add_custom_target(spsc_format
        COMMAND "${CMAKE_COMMAND}" -E echo
            "clang-format will modify: ${SPSC_FORMAT_SOURCES}"
        COMMAND "${CLANG_FORMAT_EXECUTABLE}"
            "--style=file:${PROJECT_SOURCE_DIR}/clang-format"
            -i
            --verbose
            ${SPSC_FORMAT_SOURCES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Formatting C++ sources with clang-format"
        VERBATIM
    )
else()
    message(WARNING "clang-format was not found; format targets are unavailable.")
endif()
