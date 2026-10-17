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
