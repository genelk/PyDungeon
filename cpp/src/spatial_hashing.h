#pragma once

#include <unordered_map>
#include <vector>
#include <functional>
#include "room.h"

namespace pydungeon {

/**
 * @brief SpatialHashing provides efficient spatial partitioning for room collision detection
 * 
 * The SpatialHashing class divides the space into a grid of cells and stores which rooms
 * overlap with each cell. This allows for efficient collision detection by only checking
 * rooms that are in the same or neighboring cells.
 */
class SpatialHashing {
public:
    /**
     * @brief Construct a new Spatial Hashing object
     * 
     * @param cellSize The size of each cell in the grid
     * @param worldWidth The width of the world
     * @param worldHeight The height of the world
     */
    SpatialHashing(int cellSize, int worldWidth, int worldHeight);

    /**
     * @brief Insert a room into the spatial hash
     * 
     * @param room The room to insert
     * @param roomIndex The index of the room
     */
    void insert(const Room& room, size_t roomIndex);

    /**
     * @brief Clear all rooms from the spatial hash
     */
    void clear();

    /**
     * @brief Check if a room overlaps with any existing rooms
     * 
     * @param room The room to check
     * @param padding Optional padding around rooms
     * @return std::vector<size_t> Indices of rooms that overlap with the given room
     */
    std::vector<size_t> queryOverlaps(const Room& room, int padding = 1) const;

    /**
     * @brief Get all room indices that exist in cells that overlap with the given room
     * 
     * @param room The room to check
     * @return std::vector<size_t> Indices of rooms that might overlap
     */
    std::vector<size_t> getPotentialOverlaps(const Room& room) const;

private:
    int m_cellSize;
    int m_worldWidth;
    int m_worldHeight;
    std::unordered_map<int, std::vector<size_t>> m_cells;

    /**
     * @brief Get the cell index for a given position
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @return int The cell index
     */
    int getCellIndex(int x, int y) const;

    /**
     * @brief Get all cell indices that a room overlaps with
     * 
     * @param room The room
     * @return std::vector<int> The cell indices
     */
    std::vector<int> getCellIndices(const Room& room) const;
};

} // namespace pydungeon
