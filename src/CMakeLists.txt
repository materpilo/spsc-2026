add_library(spsc
    spsc.cpp
    spsc.hpp
)
add_library(spsc::spsc ALIAS spsc)

target_include_directories(spsc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(spsc PRIVATE
    ${SPSC_WARNING_OPTIONS}
    -O3
    -DNDEBUG
)

if(BUILD_TESTING)
    target_compile_options(spsc PRIVATE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
endif()
