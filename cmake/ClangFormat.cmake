find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-20 clang-format-19 clang-format-18
        clang-format-17 clang-format-16 clang-format-15 clang-format-14
)

if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(spsc_format_check)
    add_custom_target(spsc_format)

    function(spsc_add_format_targets component config_file)
        set(sources ${ARGN})
        string(JOIN ", " source_list ${sources})

        add_custom_target(${component}_format_check
            COMMAND "${CMAKE_COMMAND}" -E echo
                "clang-format will check: ${source_list}"
            COMMAND "${CLANG_FORMAT_EXECUTABLE}"
                "--style=file:${config_file}"
                --dry-run
                --Werror
                --verbose
                ${sources}
            COMMENT "Checking C++ formatting for ${component}"
            VERBATIM
        )
        add_dependencies(spsc_format_check ${component}_format_check)

        add_custom_target(${component}_format
            COMMAND "${CMAKE_COMMAND}" -E echo
                "clang-format will modify: ${source_list}"
            COMMAND "${CLANG_FORMAT_EXECUTABLE}"
                "--style=file:${config_file}"
                -i
                --verbose
                ${sources}
            COMMENT "Formatting C++ sources for ${component}"
            VERBATIM
        )
        add_dependencies(spsc_format ${component}_format)
    endfunction()
else()
    message(WARNING "clang-format was not found; format targets are unavailable.")
endif()
