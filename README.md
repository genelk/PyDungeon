# PyDungeon

A Python-based procedural dungeon generator with a high-performance C++ core, inspired by Diablo's gothic aesthetic.

![PyDungeon Screenshot](https://github.com/user-attachments/assets/104a5fab-4df8-4b15-bd14-3fb26ec1f08e)

## Features

- High-performance dungeon generation using C++ with Python fallback
- Procedural generation of dungeon layouts with customizable parameters
- Visualization of dungeons using Pygame
- L-shaped corridor generation connecting rooms
- Multi-threaded room placement for larger dungeons
- Spatial partitioning for efficient collision detection
- Benchmarking tools to compare C++ and Python performance

## C++ Core Engine

PyDungeon uses a high-performance C++ core for dungeon generation algorithms, with Python handling visualization and user interaction. This hybrid architecture provides significant performance benefits while maintaining the ease of use and rapid development of Python.

### Performance Benefits

The C++ implementation provides significant performance improvements:

| Dungeon Size | Python Only | C++ Core | Speedup |
|--------------|-------------|----------|---------|
| 50x50        | 1.2 sec     | 0.08 sec | 15x     |
| 100x100      | 5.8 sec     | 0.24 sec | 24x     |
| 200x200      | 28.4 sec    | 0.9 sec  | 31x     |

### Implementation Details

The core algorithms have been rewritten in C++ with several optimizations:

- **Multi-threading**: Room placement algorithms use multiple threads for parallel processing
- **Spatial hashing**: Fast collision detection using spatial partitioning
- **Custom memory management**: Optimized grid operations
- **Efficient pathfinding**: Optimized corridor generation

This hybrid approach demonstrates how to combine the performance benefits of C++ with the ease of use and visualization capabilities of Python.

## Project Structure

```
pydungeon/
├── cpp/
│   ├── src/
│   │   ├── dungeon.cpp       # Core generation algorithms
│   │   ├── dungeon.h
│   │   ├── room.cpp          # Room placement logic
│   │   ├── room.h
│   │   ├── corridor.cpp      # Corridor generation
│   │   ├── corridor.h
│   │   ├── spatial_hashing.cpp  # Spatial partitioning
│   │   └── spatial_hashing.h
│   ├── binding/
│   │   └── pybind.cpp        # Python bindings
│   ├── CMakeLists.txt        # Build configuration
│   └── tests/                # C++ unit tests
├── pydungeon/
│   ├── __init__.py
│   ├── visualization.py      # Pygame visualization
│   └── dungeon_wrapper.py    # Python wrapper for C++ module
├── main.py                   # Entry point
└── setup.py                  # Build script
```

## Installation

### Prerequisites

- Python 3.6+
- CMake 3.10+
- C++ compiler with C++17 support (GCC 7+, Clang 5+, MSVC 2017+)
- Pygame 2.0+
- NumPy 1.19+
- pybind11 2.6+

### Building from Source

```bash
# Install required tools
sudo apt install cmake g++  # For Ubuntu/Debian
# or
brew install cmake          # For macOS

# Clone the repository
git clone https://github.com/username/pydungeon.git
cd pydungeon

# Install with pip (this will build the C++ extension)
pip install -e .
```

## Usage

### Running the Application

```bash
# Run with default settings
python main.py

# Specify dungeon size
python main.py --width 100 --height 100

# Force Python implementation (no C++)
python main.py --python

# Run benchmark
python main.py --benchmark

# Set random seed
python main.py --seed 12345
```

### Controls

- **SPACE**: Generate a new dungeon
- **B**: Run a quick benchmark
- **P**: Toggle between Python and C++ implementation
- **ESC**: Exit the application

### Using as a Library

```python
from pydungeon import DungeonGenerator, DungeonRenderer

# Create a generator
generator = DungeonGenerator(width=100, height=100)

# Generate a dungeon
dungeon, generation_time = generator.generate()

# ... visualize or process the dungeon ...
```

## My C++ Learning Journey

This project represents my journey learning C++ after having a strong Python background. Some key insights:

1. **Memory Management**: Learning to manage memory explicitly
2. **Multi-threading**: Implementing parallel algorithms
3. **Performance Optimization**: Identifying and optimizing bottlenecks
4. **C++/Python Integration**: Bridging the two languages with pybind11

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
