#pragma once

#include <vector>
#include <random>
#include <memory>
#include "room.h"
#include "corridor.h"

namespace pydungeon {

/**
 * @brief The DungeonGenerator class generates procedural dungeons inspired by Diablo's gothic aesthetic.
 * 
 * The generator uses a room-based approach, placing rooms and connecting
 * them with corridors to create a dungeon layout.
 */
class DungeonGenerator {
public:
    // Tile types
    enum TileType {
        EMPTY = 0,
        WALL = 1,
        FLOOR = 2,
        CORRIDOR = 3,
        DOOR = 4
    };

    /**
     * @brief Construct a new Dungeon Generator object
     * 
     * @param width Width of the dungeon grid
     * @param height Height of the dungeon grid
     * @param minRooms Minimum number of rooms to generate
     * @param maxRooms Maximum number of rooms to generate
     * @param minRoomSize Minimum size (width/height) of rooms
     * @param maxRoomSize Maximum size (width/height) of rooms
     * @param seed Random seed (optional)
     */
    DungeonGenerator(
        int width = 50, 
        int height = 50, 
        int minRooms = 5, 
        int maxRooms = 15, 
        int minRoomSize = 5, 
        int maxRoomSize = 15,
        unsigned int seed = std::random_device{}()
    );

    /**
     * @brief Generate a new dungeon layout
     * 
     * @return std::vector<std::vector<int>> 2D vector representing the dungeon layout
     */
    std::vector<std::vector<int>> generate();

    /**
     * @brief Get the dungeon width
     * 
     * @return int The width of the dungeon
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Get the dungeon height
     * 
     * @return int The height of the dungeon
     */
    int getHeight() const { return m_height; }

private:
    int m_width;
    int m_height;
    int m_minRooms;
    int m_maxRooms;
    int m_minRoomSize;
    int m_maxRoomSize;
    std::mt19937 m_rng;  // Mersenne Twister random number generator

    /**
     * @brief Create a rectangular room in the dungeon
     * 
     * @param dungeon The dungeon grid
     * @param room The room to create
     */
    void createRoom(std::vector<std::vector<int>>& dungeon, const Room& room);

    /**
     * @brief Create a corridor between two points
     * 
     * @param dungeon The dungeon grid
     * @param corridor The corridor to create
     */
    void createCorridor(std::vector<std::vector<int>>& dungeon, const Corridor& corridor);
};

} // namespace pydungeon
