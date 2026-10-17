find_program(CLANG_TIDY_EXECUTABLE
    NAMES clang-tidy clang-tidy-20 clang-tidy-19 clang-tidy-18
        clang-tidy-17 clang-tidy-16 clang-tidy-15 clang-tidy-14
)

if(CLANG_TIDY_EXECUTABLE)
    add_custom_target(spsc_tidy_check)

    function(spsc_add_tidy_check component config_file)
        set(sources ${ARGN})
        string(JOIN ", " source_list ${sources})

        add_custom_target(${component}_tidy_check
            COMMAND "${CMAKE_COMMAND}" -E echo
                "clang-tidy will analyze: ${source_list}"
            COMMAND "${CLANG_TIDY_EXECUTABLE}"
                "--config-file=${config_file}"
                -p "${CMAKE_BINARY_DIR}"
                ${sources}
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Running clang-tidy for ${component}"
            VERBATIM
        )
        add_dependencies(spsc_tidy_check ${component}_tidy_check)
    endfunction()

    function(spsc_add_tidy_header_check component config_file header_file include_directory)
        add_custom_target(${component}_tidy_check
            COMMAND "${CMAKE_COMMAND}" -E echo
                "clang-tidy will analyze: ${header_file}"
            COMMAND "${CLANG_TIDY_EXECUTABLE}"
                "--config-file=${config_file}"
                "${header_file}"
                --
                "-std=c++${CMAKE_CXX_STANDARD}"
                "-I${include_directory}"
            WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
            COMMENT "Running clang-tidy for ${component}"
            VERBATIM
        )
        add_dependencies(spsc_tidy_check ${component}_tidy_check)
    endfunction()
else()
    message(WARNING "clang-tidy was not found; tidy-check targets are unavailable.")
endif()
