add_library(spsc INTERFACE)
add_library(spsc::spsc ALIAS spsc)

target_sources(spsc
    INTERFACE
    spsc.hpp
)

target_include_directories(spsc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(CLANG_TIDY_EXECUTABLE)
    spsc_add_tidy_header_check(spsc_library "${SPSC_CLANG_TIDY_CONFIG}"
        "${CMAKE_CURRENT_SOURCE_DIR}/spsc.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endif()

if(CLANG_FORMAT_EXECUTABLE)
    spsc_add_format_targets(spsc_library "${SPSC_CLANG_FORMAT_CONFIG}"
        "${CMAKE_CURRENT_SOURCE_DIR}/spsc.hpp"
    )
endif()
