# Dungeon Generator

A C++ procedural dungeon generator with Python visualization, inspired by classic dungeon crawler games like Diablo.

## Project Overview

This project consists of two main components:
1. A C++ application that generates procedural dungeons and saves them to a file
2. A Python script that visualizes the generated dungeons

This architecture leverages the performance benefits of C++ for the generation algorithms while using Python for easy visualization.

## Features

- Procedural generation of dungeon layouts with customizable parameters
- Random room placement and connecting corridors
- Configurable dungeon size, room count, and room dimensions
- Simple file format for exporting dungeon layouts
- Visualization using Python and Pygame
![Screenshot 2025-03-17 180227](https://github.com/user-attachments/assets/4b6ed3dd-b256-495d-aa75-30f2670797e2)



## Requirements

### C++ Component
- C++17 compatible compiler (MSVC, GCC, or Clang)
- Visual Studio 2019/2022 or Visual Studio Code

### Python Component
- Python 3.6 or newer
- Pygame
- NumPy

## Building the Project

### Using Visual Studio
1. Open the solution file in Visual Studio
2. Build the solution (F7 or Build → Build Solution)
3. Run the C++ application to generate a dungeon

### Using Command Line
```bash
cd cpp
mkdir build
cd build
cmake ..
cmake --build .
./dungeon_generator
```

## Running the Visualization

After generating a dungeon with the C++ application:

```bash
cd python
python visualize.py
```

## Usage

1. Generate a dungeon using the C++ application
   - Default: `dungeon_generator.exe`
   - With parameters: `dungeon_generator.exe 80 60` (for 80×60 dungeon)

2. Visualize the dungeon using the Python script
   - Run: `python visualize.py`

## Implementation Details

### C++ Core
- Room placement using overlap detection
- L-shaped corridor generation between rooms
- Simple file-based serialization

### Python Visualization
- Pygame-based renderer
- Color-coded tile types
- Centered display with automatic scaling

## License

This project is licensed under the MIT License - see the LICENSE file for details.
