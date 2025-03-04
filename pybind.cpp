#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "../src/dungeon.h"

namespace py = pybind11;

/**
 * Convert a C++ 2D vector to a NumPy array for Python
 */
py::array_t<int> dungeonToNumpy(const std::vector<std::vector<int>>& dungeon) {
    if (dungeon.empty()) {
        return py::array_t<int>();
    }
    
    size_t height = dungeon.size();
    size_t width = dungeon[0].size();
    
    // Create a NumPy array with the same shape as the dungeon
    py::array_t<int> result({height, width});
    auto buffer = result.request();
    int* ptr = static_cast<int*>(buffer.ptr);
    
    // Copy data from the 2D vector to the NumPy array
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            ptr[i * width + j] = dungeon[i][j];
        }
    }
    
    return result;
}

/**
 * Benchmark function to compare Python vs C++ performance
 */
py::dict benchmark_dungeon_generation(int width, int height, int num_iterations = 5) {
    pydungeon::DungeonGenerator generator(width, height);
    
    double total_time = 0.0;
    
    for (int i = 0; i < num_iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        generator.generate();
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double> diff = end - start;
        total_time += diff.count();
    }
    
    double avg_time = total_time / num_iterations;
    
    py::dict result;
    result["width"] = width;
    result["height"] = height;
    result["iterations"] = num_iterations;
    result["avg_time"] = avg_time;
    
    return result;
}

PYBIND11_MODULE(pydungeon_cpp, m) {
    m.doc() = "PyDungeon C++ extension module";
    
    // Expose the tile type constants
    py::enum_<pydungeon::DungeonGenerator::TileType>(m, "TileType")
        .value("EMPTY", pydungeon::DungeonGenerator::EMPTY)
        .value("WALL", pydungeon::DungeonGenerator::WALL)
        .value("FLOOR", pydungeon::DungeonGenerator::FLOOR)
        .value("CORRIDOR", pydungeon::DungeonGenerator::CORRIDOR)
        .value("DOOR", pydungeon::DungeonGenerator::DOOR)
        .export_values();
    
    // Expose the DungeonGenerator class
    py::class_<pydungeon::DungeonGenerator>(m, "DungeonGenerator")
        .def(py::init<int, int, int, int, int, int, unsigned int>(),
            py::arg("width") = 50,
            py::arg("height") = 50,
            py::arg("min_rooms") = 5,
            py::arg("max_rooms") = 15,
            py::arg("min_room_size") = 5,
            py::arg("max_room_size") = 15,
            py::arg("seed") = 0)
        .def("generate", [](pydungeon::DungeonGenerator& self) {
            // Start timing
            auto start = std::chrono::high_resolution_clock::now();
            
            // Generate the dungeon
            auto dungeon = self.generate();
            
            // End timing
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            
            // Convert the dungeon to a NumPy array
            auto numpy_dungeon = dungeonToNumpy(dungeon);
            
            // Create a dictionary to return both the dungeon and the time it took
            py::dict result;
            result["dungeon"] = numpy_dungeon;
            result["time"] = diff.count();
            
            return result;
        })
        .def("get_width", &pydungeon::DungeonGenerator::getWidth)
        .def("get_height", &pydungeon::DungeonGenerator::getHeight);
        
    // Add the benchmark function
    m.def("benchmark", &benchmark_dungeon_generation,
          py::arg("width"), py::arg("height"), py::arg("num_iterations") = 5,
          "Benchmark the dungeon generation performance");
