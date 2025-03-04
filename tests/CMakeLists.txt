cmake_minimum_required(VERSION 3.10)

# Google Test dependency
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Tests source files
set(TEST_SOURCES
    test_room.cpp
    test_corridor.cpp
    test_dungeon.cpp
    test_spatial_hashing.cpp
)

# Create test executable
add_executable(pydungeon_tests ${TEST_SOURCES})

# Link against the Google Test libraries
target_link_libraries(pydungeon_tests PRIVATE
    gtest
    gtest_main
    Threads::Threads
)

# Include directories
target_include_directories(pydungeon_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Add the test to CTest
add_test(NAME pydungeon_tests COMMAND pydungeon_tests)
