cmake_minimum_required(VERSION 3.10)
project(dungeon_generator VERSION 1.0)

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

# Source files
set(SOURCES
    main.cpp
    dungeon.cpp
    room.cpp
)

# Add executable
add_executable(dungeon_generator ${SOURCES})

# Install target
install(TARGETS dungeon_generator
    RUNTIME DESTINATION bin
)
