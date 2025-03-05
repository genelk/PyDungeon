#pragma once
#include <vector>
#include <string>
#include <random>
#include "room.h"

class Dungeon {
public:
    // Tile types for the dungeon
    enum TileType {
        EMPTY = 0,
        WALL = 1,
        FLOOR = 2,
        CORRIDOR = 3,
        DOOR = 4
    };

    /**
     * @brief Construct a new Dungeon
     * 
     * @param width Width of the dungeon
     * @param height Height of the dungeon
     * @param seed Random seed (0 for random)
     */
    Dungeon(int width = 50, int height = 50, unsigned seed = 0);
    
    /**
     * @brief Generate a new dungeon layout
     * 
     * @param minRooms Minimum number of rooms
     * @param maxRooms Maximum number of rooms
     * @param minRoomSize Minimum room size
     * @param maxRoomSize Maximum room size
     */
    void generate(int minRooms = 5, int maxRooms = 15, 
                  int minRoomSize = 5, int maxRoomSize = 15);
    
    /**
     * @brief Save the dungeon to a file
     * 
     * @param filename Name of the file to save to
     * @return true if successful, false otherwise
     */
    bool saveToFile(const std::string& filename) const;
    
    /**
     * @brief Get the width of the dungeon
     */
    int getWidth() const { return width; }
    
    /**
     * @brief Get the height of the dungeon
     */
    int getHeight() const { return height; }
    
    /**
     * @brief Get the value of a tile at the specified position
     * 
     * @param x X-coordinate
     * @param y Y-coordinate
     * @return Tile type at the position
     */
    int getTile(int x, int y) const;

private:
    int width;
    int height;
    std::vector<std::vector<int>> grid;
    std::vector<Room> rooms;
    std::mt19937 rng; // Random number generator
    
    /**
     * @brief Initialize the grid with walls
     */
    void initializeGrid();
    
    /**
     * @brief Place a room in the dungeon
     * 
     * @param room The room to place
     */
    void placeRoom(const Room& room);
    
    /**
     * @brief Create an L-shaped corridor between two points
     * 
     * @param x1 Start X-coordinate
     * @param y1 Start Y-coordinate
     * @param x2 End X-coordinate
     * @param y2 End Y-coordinate
     */
    void createCorridor(int x1, int y1, int x2, int y2);
    
    /**
     * @brief Create a horizontal corridor
     * 
     * @param x1 Start X-coordinate
     * @param x2 End X-coordinate
     * @param y Y-coordinate
     */
    void createHorizontalCorridor(int x1, int x2, int y);
    
    /**
     * @brief Create a vertical corridor
     * 
     * @param y1 Start Y-coordinate
     * @param y2 End Y-coordinate
     * @param x X-coordinate
     */
    void createVerticalCorridor(int y1, int y2, int x);
};
