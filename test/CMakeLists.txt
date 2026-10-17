add_executable(spsc_tests
    spsc_test.cpp
)
target_link_libraries(spsc_tests PRIVATE spsc::spsc GTest::gtest_main)
target_compile_options(spsc_tests PRIVATE
    ${SPSC_WARNING_OPTIONS}
    -O0
    -g
    -UNDEBUG
    -fsanitize=address,undefined
    -fno-omit-frame-pointer
)
target_link_options(spsc_tests PRIVATE
    -fsanitize=address,undefined
    -fno-omit-frame-pointer
)

include(GoogleTest)
gtest_discover_tests(spsc_tests)

if(CLANG_TIDY_EXECUTABLE)
    spsc_add_tidy_check(spsc_tests "${SPSC_CLANG_TIDY_CONFIG}"
        "${CMAKE_CURRENT_SOURCE_DIR}/spsc_test.cpp"
    )
endif()

if(CLANG_FORMAT_EXECUTABLE)
    spsc_add_format_targets(spsc_tests "${SPSC_CLANG_FORMAT_CONFIG}"
        "${CMAKE_CURRENT_SOURCE_DIR}/spsc_test.cpp"
    )
endif()
