#include "dungeon.h"
#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>

Dungeon::Dungeon(int width, int height, unsigned seed)
    : width(width), height(height) {
    
    // Initialize random number generator
    if (seed == 0) {
        std::random_device rd;
        seed = rd();
    }
    rng = std::mt19937(seed);
    
    // Initialize grid
    initializeGrid();
}

void Dungeon::initializeGrid() {
    // Initialize grid with walls
    grid.resize(height, std::vector<int>(width, WALL));
}

void Dungeon::generate(int minRooms, int maxRooms, int minRoomSize, int maxRoomSize) {
    // Clear previous generation
    rooms.clear();
    initializeGrid();
    
    // Generate random number of rooms
    std::uniform_int_distribution<int> roomCountDist(minRooms, maxRooms);
    int numRooms = roomCountDist(rng);
    
    // Generate rooms
    const int maxAttempts = numRooms * 10; // Limit number of attempts
    int attempts = 0;
    
    while (rooms.size() < numRooms && attempts < maxAttempts) {
        attempts++;
        
        // Generate random room dimensions
        std::uniform_int_distribution<int> roomWidthDist(minRoomSize, maxRoomSize);
        std::uniform_int_distribution<int> roomHeightDist(minRoomSize, maxRoomSize);
        
        int roomWidth = roomWidthDist(rng);
        int roomHeight = roomHeightDist(rng);
        
        // Generate random room position
        std::uniform_int_distribution<int> xDist(1, width - roomWidth - 2);
        std::uniform_int_distribution<int> yDist(1, height - roomHeight - 2);
        
        int x = xDist(rng);
        int y = yDist(rng);
        
        Room newRoom(x, y, roomWidth, roomHeight);
        
        // Check if the room overlaps with existing rooms
        bool overlaps = false;
        for (const auto& existingRoom : rooms) {
            if (newRoom.overlaps(existingRoom)) {
                overlaps = true;
                break;
            }
        }
        
        // If room doesn't overlap, add it to our collection
        if (!overlaps) {
            rooms.push_back(newRoom);
            placeRoom(newRoom);
        }
    }
    
    // Connect rooms with corridors
    if (rooms.size() > 1) {
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            // Get center of current room
            int x1 = rooms[i].getCenterX();
            int y1 = rooms[i].getCenterY();
            
            // Get center of next room
            int x2 = rooms[i+1].getCenterX();
            int y2 = rooms[i+1].getCenterY();
            
            // Randomly decide corridor type (horizontal-then-vertical or vertical-then-horizontal)
            std::uniform_int_distribution<int> coinFlip(0, 1);
            bool horizontalFirst = coinFlip(rng);
            
            // Create L-shaped corridor
            if (horizontalFirst) {
                createHorizontalCorridor(x1, x2, y1);
                createVerticalCorridor(y1, y2, x2);
            } else {
                createVerticalCorridor(y1, y2, x1);
                createHorizontalCorridor(x1, x2, y2);
            }
        }
    }
}

void Dungeon::placeRoom(const Room& room) {
    for (int y = room.getY(); y < room.getY() + room.getHeight(); ++y) {
        for (int x = room.getX(); x < room.getX() + room.getWidth(); ++x) {
            if (y >= 0 && y < height && x >= 0 && x < width) {
                grid[y][x] = FLOOR;
            }
        }
    }
}

void Dungeon::createCorridor(int x1, int y1, int x2, int y2) {
    // Randomly decide whether to go horizontal or vertical first
    std::uniform_int_distribution<int> coinFlip(0, 1);
    bool horizontalFirst = coinFlip(rng);
    
    if (horizontalFirst) {
        createHorizontalCorridor(x1, x2, y1);
        createVerticalCorridor(y1, y2, x2);
    } else {
        createVerticalCorridor(y1, y2, x1);
        createHorizontalCorridor(x1, x2, y2);
    }
}

void Dungeon::createHorizontalCorridor(int x1, int x2, int y) {
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            if (grid[y][x] == WALL) {
                grid[y][x] = CORRIDOR;
            }
        }
    }
}

void Dungeon::createVerticalCorridor(int y1, int y2, int x) {
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
        if (y >= 0 && y < height && x >= 0 && x < width) {
            if (grid[y][x] == WALL) {
                grid[y][x] = CORRIDOR;
            }
        }
    }
}

int Dungeon::getTile(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return EMPTY; // Default for out of bounds
}

bool Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Write dimensions
    file << width << " " << height << std::endl;
    
    // Write grid
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            file << grid[y][x];
            if (x < width - 1) {
                file << " ";
            }
        }
        file << std::endl;
    }
    
    file.close();
    return true;
}
