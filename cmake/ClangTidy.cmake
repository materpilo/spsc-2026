find_program(CLANG_TIDY_EXECUTABLE
    NAMES clang-tidy clang-tidy-20 clang-tidy-19 clang-tidy-18
        clang-tidy-17 clang-tidy-16 clang-tidy-15 clang-tidy-14
)

if(CLANG_TIDY_EXECUTABLE)
    set(SPSC_TIDY_SOURCES
        "${PROJECT_SOURCE_DIR}/src/spsc.cpp"
    )
    if(BUILD_TESTING)
        list(APPEND SPSC_TIDY_SOURCES
            "${PROJECT_SOURCE_DIR}/test/spsc_test.cpp"
        )
    endif()

    add_custom_target(spsc_tidy_check
        COMMAND "${CMAKE_COMMAND}" -E echo
            "clang-tidy will analyze: ${SPSC_TIDY_SOURCES}"
        COMMAND "${CLANG_TIDY_EXECUTABLE}"
            "--config-file=${PROJECT_SOURCE_DIR}/clang-tidy"
            -p "${CMAKE_BINARY_DIR}"
            ${SPSC_TIDY_SOURCES}
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Running clang-tidy"
        VERBATIM
    )
else()
    message(WARNING "clang-tidy was not found; target spsc_tidy_check is unavailable.")
endif()
