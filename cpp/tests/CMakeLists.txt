cmake_minimum_required(VERSION 3.10)
project(pydungeon_cpp VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler warning flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find pybind11
find_package(pybind11 REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/dungeon.cpp
    src/room.cpp
    src/corridor.cpp
    src/spatial_hashing.cpp
    binding/pybind.cpp
)

# Create the pybind11 module
pybind11_add_module(pydungeon_cpp ${SOURCES})

# Set output directory
set_target_properties(pydungeon_cpp PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../pydungeon
)

# Enable optimization for release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(pydungeon_cpp PRIVATE /O2)
    else()
        target_compile_options(pydungeon_cpp PRIVATE -O3)
    endif()
endif()

# Add multi-threading support
find_package(Threads REQUIRED)
target_link_libraries(pydungeon_cpp PRIVATE Threads::Threads)

# Install target
install(TARGETS pydungeon_cpp
    LIBRARY DESTINATION pydungeon
)

# Add testing
enable_testing()
add_subdirectory(tests)
