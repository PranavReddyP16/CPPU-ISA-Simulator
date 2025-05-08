# Enable testing
enable_testing()

# Find GoogleTest (or install it via FetchContent if not installed)
find_package(GTest REQUIRED)

# Define test source files
set(TEST_SOURCES
    test_general_cache.cpp
    test_memory.cpp
    test_string_cache.cpp
)

# Create a test executable
add_executable(isa_tests ${TEST_SOURCES})

# Include the ISA core headers
target_include_directories(isa_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link the test executable against the ISA core library and GoogleTest
target_link_libraries(isa_tests PRIVATE isa_core GTest::GTest GTest::Main)

# Add test execution
add_test(NAME ISA_Tests COMMAND isa_tests)
