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
