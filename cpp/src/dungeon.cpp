#include "dungeon.h"
#include <algorithm>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>

namespace pydungeon {

DungeonGenerator::DungeonGenerator(
    int width, int height, int minRooms, int maxRooms, 
    int minRoomSize, int maxRoomSize, unsigned int seed)
    : m_width(width), m_height(height), 
      m_minRooms(minRooms), m_maxRooms(maxRooms),
      m_minRoomSize(minRoomSize), m_maxRoomSize(maxRoomSize),
      m_rng(seed) {}

std::vector<std::vector<int>> DungeonGenerator::generate() {
    // Initialize dungeon with walls
    std::vector<std::vector<int>> dungeon(
        m_height, std::vector<int>(m_width, WALL)
    );
    
    // List to store rooms
    std::vector<Room> rooms;
    
    // Generate random number of rooms
    std::uniform_int_distribution<int> roomCountDist(m_minRooms, m_maxRooms);
    int numRooms = roomCountDist(m_rng);
    
    // Generate rooms
    const int maxAttempts = numRooms * 10; // Limit number of attempts
    int attempts = 0;
    
    // Use multi-threading for room placement if we need many rooms
    const bool useMultiThreading = (numRooms > 10 && m_width > 100 && m_height > 100);
    
    if (useMultiThreading) {
        // Parallelize room generation using futures
        std::vector<std::future<Room>> roomFutures;
        std::mutex roomsMutex;
        
        const int threadsToUse = std::min(numRooms, static_cast<int>(std::thread::hardware_concurrency()));
        
        for (int i = 0; i < threadsToUse; ++i) {
            roomFutures.push_back(std::async(std::launch::async, [this, &rooms, &roomsMutex, numRooms, i, threadsToUse]() {
                // Each thread gets its own random generator with a different seed
                std::mt19937 localRng(m_rng() + i);
                
                // Generate room dimensions and positions
                std::uniform_int_distribution<int> roomWidthDist(m_minRoomSize, m_maxRoomSize);
                std::uniform_int_distribution<int> roomHeightDist(m_minRoomSize, m_maxRoomSize);
                
                int roomsPerThread = numRooms / threadsToUse;
                int extraRooms = numRooms % threadsToUse;
                int roomsToGenerate = roomsPerThread + (i < extraRooms ? 1 : 0);
                
                for (int r = 0; r < roomsToGenerate; ++r) {
                    int roomWidth = roomWidthDist(localRng);
                    int roomHeight = roomHeightDist(localRng);
                    
                    // Generate random room position
                    std::uniform_int_distribution<int> xDist(1, m_width - roomWidth - 1);
                    std::uniform_int_distribution<int> yDist(1, m_height - roomHeight - 1);
                    
                    int x = xDist(localRng);
                    int y = yDist(localRng);
                    
                    Room newRoom(x, y, roomWidth, roomHeight);
                    
                    // Check if the room overlaps with existing rooms
                    bool overlaps = false;
                    {
                        std::lock_guard<std::mutex> lock(roomsMutex);
                        for (const auto& existingRoom : rooms) {
                            if (newRoom.overlaps(existingRoom)) {
                                overlaps = true;
                                break;
                            }
                        }
                        
                        // If room doesn't overlap, add it to our collection
                        if (!overlaps) {
                            rooms.push_back(newRoom);
                        }
                    }
                    
                    if (!overlaps) {
                        return newRoom; // Success
                    }
                }
                
                // If we couldn't place a room, return a dummy one
                return Room(0, 0, 0, 0);
            }));
        }
        
        // Collect results from futures
        for (auto& future : roomFutures) {
            Room room = future.get();
            if (room.getWidth() > 0) {
                createRoom(dungeon, room);
            }
        }
    } else {
        // Single-threaded room generation for smaller dungeons
        while (rooms.size() < numRooms && attempts < maxAttempts) {
            attempts++;
            
            // Generate random room dimensions
            std::uniform_int_distribution<int> roomWidthDist(m_minRoomSize, m_maxRoomSize);
            std::uniform_int_distribution<int> roomHeightDist(m_minRoomSize, m_maxRoomSize);
            
            int roomWidth = roomWidthDist(m_rng);
            int roomHeight = roomHeightDist(m_rng);
            
            // Generate random room position
            std::uniform_int_distribution<int> xDist(1, m_width - roomWidth - 1);
            std::uniform_int_distribution<int> yDist(1, m_height - roomHeight - 1);
            
            int x = xDist(m_rng);
            int y = yDist(m_rng);
            
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
                createRoom(dungeon, newRoom);
            }
        }
    }
    
    // Connect rooms with corridors
    if (rooms.size() > 1) {
        std::uniform_int_distribution<int> corridorTypeDist(0, 1);
        
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            // Get center of current room
            int x1 = rooms[i].getCenterX();
            int y1 = rooms[i].getCenterY();
            
            // Get center of next room
            int x2 = rooms[i+1].getCenterX();
            int y2 = rooms[i+1].getCenterY();
            
            // Randomly decide corridor type
            Corridor::CorridorType type = corridorTypeDist(m_rng) == 0 
                ? Corridor::HORIZONTAL_THEN_VERTICAL 
                : Corridor::VERTICAL_THEN_HORIZONTAL;
            
            // Create L-shaped corridor
            Corridor corridor(x1, y1, x2, y2, type);
            createCorridor(dungeon, corridor);
        }
    }
    
    return dungeon;
}

void DungeonGenerator::createRoom(std::vector<std::vector<int>>& dungeon, const Room& room) {
    for (int y = room.getY(); y < room.getY() + room.getHeight(); ++y) {
        for (int x = room.getX(); x < room.getX() + room.getWidth(); ++x) {
            if (y >= 0 && y < m_height && x >= 0 && x < m_width) {
                dungeon[y][x] = FLOOR;
            }
        }
    }
}

void DungeonGenerator::createCorridor(std::vector<std::vector<int>>& dungeon, const Corridor& corridor) {
    int x1 = corridor.getStartX();
    int y1 = corridor.getStartY();
    int x2 = corridor.getEndX();
    int y2 = corridor.getEndY();
    
    // Get the corner point where the corridor turns
    auto [cornerX, cornerY] = corridor.getCornerPoint();
    
    // Draw the first segment of the corridor
    if (corridor.getType() == Corridor::HORIZONTAL_THEN_VERTICAL) {
        // Horizontal first
        for (int x = std::min(x1, cornerX); x <= std::max(x1, cornerX); ++x) {
            if (y1 >= 0 && y1 < m_height && x >= 0 && x < m_width) {
                if (dungeon[y1][x] == WALL) {
                    dungeon[y1][x] = CORRIDOR;
                }
            }
        }
        
        // Then vertical
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            if (y >= 0 && y < m_height && cornerX >= 0 && cornerX < m_width) {
                if (dungeon[y][cornerX] == WALL) {
                    dungeon[y][cornerX] = CORRIDOR;
                }
            }
        }
    } else {
        // Vertical first
        for (int y = std::min(y1, cornerY); y <= std::max(y1, cornerY); ++y) {
            if (y >= 0 && y < m_height && x1 >= 0 && x1 < m_width) {
                if (dungeon[y][x1] == WALL) {
                    dungeon[y][x1] = CORRIDOR;
                }
            }
        }
        
        // Then horizontal
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
            if (cornerY >= 0 && cornerY < m_height && x >= 0 && x < m_width) {
                if (dungeon[cornerY][x] == WALL) {
                    dungeon[cornerY][x] = CORRIDOR;
                }
            }
        }
    }
}

} // namespace pydungeon
