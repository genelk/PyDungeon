#include <iostream>
#include <string>
#include "dungeon.h"

int main(int argc, char* argv[]) {
    int width = 50;
    int height = 50;
    int minRooms = 5;
    int maxRooms = 15;
    int minRoomSize = 5;
    int maxRoomSize = 15;
    unsigned seed = 0; // 0 means use random seed
    std::string outputFile = "dungeon.txt";

    // Parse command line arguments
    if (argc > 2) {
        try {
            width = std::stoi(argv[1]);
            height = std::stoi(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing width/height: " << e.what() << std::endl;
            return 1;
        }
    }

    // Additional optional parameters
    if (argc > 4) {
        try {
            minRooms = std::stoi(argv[3]);
            maxRooms = std::stoi(argv[4]);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing room count parameters: " << e.what() << std::endl;
        }
    }

    if (argc > 6) {
        try {
            minRoomSize = std::stoi(argv[5]);
            maxRoomSize = std::stoi(argv[6]);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing room size parameters: " << e.what() << std::endl;
        }
    }

    if (argc > 7) {
        try {
            seed = static_cast<unsigned>(std::stoi(argv[7]));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing seed: " << e.what() << std::endl;
        }
    }

    if (argc > 8) {
        outputFile = argv[8];
    }

    // Validate parameters
    if (width <= 0 || height <= 0 || minRooms <= 0 || maxRooms <= 0 || 
        minRoomSize <= 0 || maxRoomSize <= 0) {
        std::cerr << "Invalid parameters: all dimensions must be positive" << std::endl;
        return 1;
    }

    if (minRooms > maxRooms) {
        std::swap(minRooms, maxRooms);
    }

    if (minRoomSize > maxRoomSize) {
        std::swap(minRoomSize, maxRoomSize);
    }

    // Print generation parameters
    std::cout << "Generating dungeon with parameters:" << std::endl;
    std::cout << "  Size: " << width << "x" << height << std::endl;
    std::cout << "  Rooms: " << minRooms << "-" << maxRooms << std::endl;
    std::cout << "  Room size: " << minRoomSize << "-" << maxRoomSize << std::endl;
    std::cout << "  Seed: " << (seed == 0 ? "random" : std::to_string(seed)) << std::endl;
    std::cout << "  Output file: " << outputFile << std::endl;

    // Generate the dungeon
    Dungeon dungeon(width, height, seed);
    dungeon.generate(minRooms, maxRooms, minRoomSize, maxRoomSize);

    // Save to file
    if (dungeon.saveToFile(outputFile)) {
        std::cout << "Dungeon generated and saved to " << outputFile << std::endl;
    } else {
        std::cerr << "Failed to save dungeon to " << outputFile << std::endl;
        return 1;
    }

    return 0;
}
