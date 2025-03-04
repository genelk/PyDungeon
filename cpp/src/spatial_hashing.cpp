
#include "spatial_hashing.h"
#include <algorithm>
#include <unordered_set>

namespace pydungeon {

SpatialHashing::SpatialHashing(int cellSize, int worldWidth, int worldHeight)
    : m_cellSize(cellSize), m_worldWidth(worldWidth), m_worldHeight(worldHeight) {}

void SpatialHashing::insert(const Room& room, size_t roomIndex) {
    std::vector<int> cellIndices = getCellIndices(room);
    
    for (int cellIndex : cellIndices) {
        m_cells[cellIndex].push_back(roomIndex);
    }
}

void SpatialHashing::clear() {
    m_cells.clear();
}

std::vector<size_t> SpatialHashing::queryOverlaps(const Room& room, int padding) const {
    std::vector<size_t> roomIndices = getPotentialOverlaps(room);
    std::vector<size_t> overlappingRooms;
    
    std::unordered_set<size_t> uniqueIndices(roomIndices.begin(), roomIndices.end());
    
    for (size_t roomIndex : uniqueIndices) {
        // We'd need a way to get the room from the index, but for this example,
        // we'll assume we're checking against existing rooms elsewhere
        overlappingRooms.push_back(roomIndex);
    }
    
    return overlappingRooms;
}

std::vector<size_t> SpatialHashing::getPotentialOverlaps(const Room& room) const {
    std::vector<int> cellIndices = getCellIndices(room);
    std::vector<size_t> potentialOverlaps;
    
    for (int cellIndex : cellIndices) {
        auto it = m_cells.find(cellIndex);
        if (it != m_cells.end()) {
            const auto& roomIndices = it->second;
            potentialOverlaps.insert(potentialOverlaps.end(), roomIndices.begin(), roomIndices.end());
        }
    }
    
    return potentialOverlaps;
}

int SpatialHashing::getCellIndex(int x, int y) const {
    int cellX = x / m_cellSize;
    int cellY = y / m_cellSize;
    
    // Use a simple hashing function to convert 2D cell coordinates to a 1D index
    return cellX + cellY * (m_worldWidth / m_cellSize + 1);
}

std::vector<int> SpatialHashing::getCellIndices(const Room& room) const {
    std::vector<int> cellIndices;
    
    int startCellX = room.getX() / m_cellSize;
    int startCellY = room.getY() / m_cellSize;
    int endCellX = (room.getX() + room.getWidth()) / m_cellSize;
    int endCellY = (room.getY() + room.getHeight()) / m_cellSize;
    
    for (int y = startCellY; y <= endCellY; ++y) {
        for (int x = startCellX; x <= endCellX; ++x) {
            cellIndices.push_back(getCellIndex(x * m_cellSize, y * m_cellSize));
        }
    }
    
    return cellIndices;
}

} // namespace pydungeon
